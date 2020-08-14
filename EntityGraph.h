#ifndef ENTITYGRAPH_H
#define ENTITYGRAPH_H

#include <map>

class World;
class Entity;

struct EntityGraph
{
  std::map <Entity*, Entity*> m;
  void update (World & w);
};


#endif // ENTITYGRAPH_H


