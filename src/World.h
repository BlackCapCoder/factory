#ifndef WORLD_H
#define WORLD_H

#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"
#include "EntityGraph.h"
#include "WorldMap.h"
#include "QuadTree.h"

class Miner;
class Belt;
class Splitter;
class Trash;
class Undergroundee;
class Base;

struct World
{
  EntityList<Belt> bs;
  EntityList<Entity>        es;
  EntityList<Undergroundee> us;

  EntityGraph               gr {*this};
  Base *                  base;
  WorldMap                 m  {};
  QuadTree<Entity>         qt {V4{-1024,-1024,2048,2048},30,20};

  World (Base * b)
  {
    base = b;
    qt.insert(reinterpret_cast<Entity*>(b));
  }


public:
  void tick   (int dt);
  void render (SDL_Renderer & r, long time, float cx, float cy, int sw, int sh);

  void add (Entity *);
  void add (Belt   *);
  void add (Undergroundee *);

  void cleanAdd (Entity *);

  void remove (Entity        *);
  void remove (Belt          *);
  void remove (Undergroundee *);

  Entity * at (int x, int y)
  {
    std::vector<Entity*> res;
    qt.getObjectsInBound(V4 { (double) x + 0.5, (double) y + 0.5, 0.1, 0.1}, res);
    if (res.empty()) return nullptr;

    return res[0];
  }
  Entity * at (V2 p) { return at (p.x, p.y); }

  void inside (int x, int y, int w, int h, std::vector<Entity*> & out)
  {
    qt.getObjectsInBound(V4 { (double) x + 0.1, (double) y + 0.1, (double) w - 0.2, (double) h - 0.2}, out);
  }
};


#endif // WORLD_H
