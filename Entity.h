#ifndef ENTITY_H
#define ENTITY_H

#include "Utils.h"
#include <vector>
#include <set>
#include <SDL2/SDL.h>
#include "QuadTree.h"


class World;

struct Entity : public Collidable<Entity>
{
  const V2 pos;
  const V2 size;

  Entity (V2 pos, V2 size)
    : pos  { pos  }
    , size { size }
  {}

  inline V4 getBounds () override
  {
    return V4 { (double) pos.x, (double) pos.y, (double) size.x, (double) size.y };
  }

  virtual void render (SDL_Renderer & r, long time) = 0;
  virtual void tick   (World & w, int dt) = 0;

  // attempt to insert an item into this entity
  virtual bool input  () = 0;
};

template <class E>
class EntityList
  : public std::set<E*>
{
public:
  void add (E * e)
  {
    this->insert (e);
  }

  E * at (int x, int y)
  {
    for (E * e : *this)
    {
      if (e->pos.x == x && e->pos.y == y)
        return e;
    }

    return nullptr;
  }

  void remove (E * e)
  {
    this->erase (e);
    delete e;
  }

  bool has (E * e)
  {
    return this->contains (e);
  }
};


#endif // ENTITY_H
