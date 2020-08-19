// https://github.com/Megabyte918/QuadTree-Cpp

#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <vector>
#include <algorithm>


struct V4
{
  double x,y,w,h;

  bool contains(const V4 &other) const noexcept
  {
    if (x > other.x) return false;
    if (y > other.y) return false;
    if (x + w < other.x + other.w) return false;
    if (y + h < other.y + other.h) return false;
    return true; // within bounds
  }

  bool intersects(const V4 &other) const noexcept
  {
    if (x > other.x + other.w) return false;
    if (x + w < other.x)       return false;
    if (y > other.y + other.h) return false;
    if (y + h < other.y)       return false;
    return true; // intersection
  }

  double getLeft()   const noexcept { return x - (w * 0.5f); }
  double getTop()    const noexcept { return y + (h * 0.5f); }
  double getRight()  const noexcept { return x + (w * 0.5f); }
  double getBottom() const noexcept { return y - (h * 0.5f); }
};

template <class> class  QuadTree;
template <class> struct Collidable;

template <class T>
struct Collidable
{
  friend class QuadTree<T>;

private:
  QuadTree<T> *qt = nullptr;

public:
  virtual V4 getBounds () = 0;
};


template <class ColT>
class QuadTree
{
private:
  bool            isLeaf = true;
  unsigned        level  = 0;
  const unsigned  capacity;
  const unsigned  maxLevel;
  const V4        bounds;
  QuadTree *      parent = nullptr;
  QuadTree *      children[4] = { nullptr, nullptr, nullptr, nullptr };
  std::vector<ColT*> objects;

private:

  // Subdivides into four quadrants
  void subdivide()
  {
    const double width  = bounds.w * 0.5f;
    const double height = bounds.h * 0.5f;
    double x = 0, y = 0;
    for (unsigned i = 0; i < 4; ++i) {
      switch (i) {
        case 0: x = bounds.x + width; y = bounds.y; break; // Top right
        case 1: x = bounds.x;         y = bounds.y; break; // Top left
        case 2: x = bounds.x;         y = bounds.y + height; break; // Bottom left
        case 3: x = bounds.x + width; y = bounds.y + height; break; // Bottom right
      }
      children[i] = new QuadTree({ x, y, width, height }, capacity, maxLevel);
      children[i]->level  = level + 1;
      children[i]->parent = this;
    }
    isLeaf = false;
  }

  // Discards buckets if all children are leaves and contain no objects
  void discardEmptyBuckets()
  {
    if (!objects.empty()) return;
    if (!isLeaf) {
      for (QuadTree *child : children)
        if (!child->isLeaf || !child->objects.empty())
          return;
    }
    if (clear(), parent != nullptr)
      parent->discardEmptyBuckets();
  }

  // Returns child that contains the provided boundary
  inline
  QuadTree * getChild(const V4 &bound) const noexcept
  {
    bool left  = bound.x + bound.w < bounds.getRight();
    bool right = bound.x > bounds.getRight();

    if (bound.y + bound.h < bounds.getTop()) {
      if (left)  return children[1]; // Top left
      if (right) return children[0]; // Top right
    } else if (bound.y > bounds.getTop()) {
      if (left)  return children[2]; // Bottom left
      if (right) return children[3]; // Bottom right
    }
    return nullptr; // Cannot contain boundary -- too large
  }

public:
  inline
  V4 getBounds () const { return bounds; }

  QuadTree () : QuadTree({}, 0, 0) {}
  QuadTree (const QuadTree &other) : QuadTree(other.bounds, other.capacity, other.maxLevel) {}

  QuadTree (const V4 &_bound, unsigned _capacity, unsigned _maxLevel)
    : bounds   (_bound)
    , capacity (_capacity)
    , maxLevel (_maxLevel)
  {
    objects.reserve (_capacity);
  }

  // Inserts an object into this quadtree
  bool insert(ColT *obj)
  {
    if (obj == nullptr || obj->qt != nullptr) return false;

    if (!isLeaf) {
      // insert object into leaf
      if (QuadTree *child = getChild(obj->getBounds()))
        return child->insert(obj);
    }
    objects.push_back(obj);
    obj->qt = this;

    // Subdivide if required
    if (isLeaf && level < maxLevel && objects.size() >= capacity) {
      subdivide();
      update(obj);
    }
    return true;
  }

  // Removes an object from this quadtree
  bool remove(ColT *obj)
  {
    if (obj == nullptr || obj->qt == nullptr) return false; // Cannot exist in vector
    if (obj->qt != this) return obj->qt->remove(obj);

    objects.erase(std::find(objects.begin(), objects.end(), obj));
    obj->qt = nullptr;
    discardEmptyBuckets();
    return true;
  }

  // Removes and re-inserts object into quadtree (for objects that move)
  bool update(ColT *obj)
  {
    if (!remove(obj)) return false;

    // Not contained in this node -- insert into parent
    if (parent != nullptr && !bounds.contains(obj->getBounds()))
      return parent->insert(obj);

    if (!isLeaf) {
      // Still within current node -- insert into leaf
      if (QuadTree *child = getChild(obj->getBounds()))
        return child->insert(obj);
    }

    return insert(obj);
  }

  // Searches quadtree for objects within the provided boundary and returns them in vector
  void getObjectsInBound (const V4 & bound, std::vector<ColT*> & foundObjects) const
  {
    for (const auto &obj : objects) {
      // Only check for intersection with OTHER boundaries
      if (&obj->getBounds() != &bound && obj->getBounds().intersects(bound))
          foundObjects.push_back(obj);
    }
    if (!isLeaf) {
      // Get objects from leaves
      if (QuadTree *child = getChild(bound)) {
        child->getObjectsInBound(bound, foundObjects);
      } else for (QuadTree *leaf : children) {
        if (leaf->bounds.intersects(bound)) {
          leaf->getObjectsInBound(bound, foundObjects);
        }
      }
    }
  }

  // Returns total children count for this quadtree
  unsigned totalChildren () const noexcept
  {
    unsigned total = 0;
    if (isLeaf) return total;
    for (QuadTree *child : children)
      total += child->totalChildren();
    return 4 + total;
  }

  // Returns total object count for this quadtree
  unsigned totalObjects () const noexcept
  {
    unsigned total = (unsigned) objects.size();
    if (!isLeaf) {
      for (QuadTree *child : children)
        total += child->totalObjects();
    }
    return total;
  }

  // Removes all objects and children from this quadtree
  void clear() noexcept
  {
    if (!objects.empty()) {
      for (auto&& obj : objects)
        obj->qt = nullptr;
      objects.clear();
    }
    if (!isLeaf) {
      for (QuadTree *child : children)
        child->clear();
      isLeaf = true;
    }
  }

  ~QuadTree()
  {
    clear();
    if (children[0]) delete children[0];
    if (children[1]) delete children[1];
    if (children[2]) delete children[2];
    if (children[3]) delete children[3];
  }
};

#endif // QUADTREE_H
