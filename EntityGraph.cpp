#include "EntityGraph.h"
#include "World.h"
#include "Utils.h"
#include "Belt.h"
#include "Miner.h"
#include "Undergroundee.h"
#include "Rotater.h"


void EntityGraph::update ()
{
  m.clear();

  for (Belt * e : w.bs)
  {
    V2 p = e->pos;

    switch (e->dout)
    {
      case (DIR::N): p.y--; break;
      case (DIR::S): p.y++; break;
      case (DIR::W): p.x--; break;
      case (DIR::E): p.x++; break;
    }

    if (auto q = w.at(p.x, p.y); q != nullptr)
    {
      if (Belt * b = dynamic_cast<Belt*>(q); b != nullptr)
      {
        if (e->dout == dswap(b->din)) m[e] = b;
      }
      else
      {
        m[e] = q;
      }
    }
  }

  for (auto * e : w.us)
  {
    if (!e->entrance) continue;
    if (e->other == nullptr) continue;

    Belt * b1 = w.bs.at (e->pos.x, e->pos.y);
    if (b1 == nullptr) continue;

    Belt * b2 = w.bs.at (e->other->pos.x, e->other->pos.y);
    if (b2 == nullptr) continue;

    m[b1] = b2;
  }
}

void EntityGraph::updateAt (int x, int y, int w, int h)
{
  std::vector<Entity*> es {};
  this->w.inside(x,y,w,h,es);
  for (auto e : es)
    if (auto q = dynamic_cast<Belt*>(e))
    {
      V2 p = q->pos + dir2V2(q->dout);
      m[e] = this->w.at (p.x, p.y);
    }
}

void EntityGraph::updateAround (Entity * e)
{
  updateAt (e->pos.x-1, e->pos.y-1, e->size.x+2, e->size.y+2);
}

