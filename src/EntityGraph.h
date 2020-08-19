#ifndef ENTITYGRAPH_H
#define ENTITYGRAPH_H

#include <map>
#include <vector>
// #include "QuadTree.h"


class World;
class Entity;

struct EntityGraph
{
  World & w;

  std::map <Entity*, Entity*> m;
  void update ();


  void updateAt (int x, int y, int w, int h);
  void updateAround (Entity * e);
};


#endif // ENTITYGRAPH_H


