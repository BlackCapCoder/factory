#ifndef MOTION_H
#define MOTION_H

#include "Game.h"
#include "World.h"
#include "Belt.h"

#include <set>
#include <vector>


namespace Motion {

#define def(N)\
  template <template <class> class V>\
  void N (World & w, V2 p, V<Entity*> & out)


using Motion
  = void(*)(World &, V2, std::vector<Entity*> & out);

template <class T>
using Vec
  = std::vector<T>;

template <class T>
struct Set : public std::set<T>
{
  void push_back (T a) { this->insert (a); }
};



def (word)
{
  if (auto b1 = dynamic_cast<Belt*>(w.at (p)))
  {
    out.push_back (b1);
    while (auto b2 = dynamic_cast<Belt*>(w.at(b1->pos + dir2V2 (b1->dout))))
    {
      if (b1->dout != dswap(b2->din)) break;
      out.push_back (b2);
      if (b1->dout != b2->dout) break;
      b1 = b2;
    }
  }
}

def (back)
{
  if (auto b1 = dynamic_cast<Belt*>(w.at (p)))
  {
    out.push_back (b1);
    while (auto b2 = dynamic_cast<Belt*>(w.at(b1->pos + dir2V2 (b1->din))))
    {
      if (b1->din != dswap(b2->dout)) break;
      out.push_back (b2);
      if (b1->din != b2->din) break;
      b1 = b2;
    }
  }
}

def (home)
{
  const V2 first { p.x, p.y };

  if (auto b1 = dynamic_cast<Belt*>(w.at (p)))
  {
    out.push_back (b1);
    while (auto b2 = dynamic_cast<Belt*>(w.at(b1->pos + dir2V2(b1->din))))
    {
      if (b1->din != dswap(b2->dout)) break;
      if (b2->pos.x == first.x && b2->pos.y == first.y) break;
      out.push_back (b2);
      b1 = b2;
    }
  }
}

def (end)
{
  const V2 first { p.x, p.y };

  if (auto b1 = dynamic_cast<Belt*>(w.at (p)))
  {
    out.push_back (b1);
    while (auto b2 = dynamic_cast<Belt*>(w.at(b1->pos + dir2V2(b1->dout))))
    {
      if (b1->dout != dswap(b2->din)) break;
      if (b2->pos.x == first.x && b2->pos.y == first.y) break;
      out.push_back (b2);
      b1 = b2;
    }
  }
}

def (line)
{
  Set<Entity*> es {};

  home (w, p, es);
  end  (w, p, es);

  for (auto e : es)
    out.push_back (e);
}



Motion constexpr Word
  = word <Vec>;

Motion constexpr Back
  = back <Vec>;

Motion constexpr Home
  = home <Vec>;

Motion constexpr End
  = end <Vec>;

Motion constexpr Line
  = line <Vec>;

#undef def

}; // namespace Motion


namespace Verb
{

#define def(N)\
  void N (Game & g, Subject subj)

#define init()\
  std::vector<Entity*> es {};\
  subj (g.world, g.cur, es);\
  if (es.empty()) return;


using Subject = Motion::Motion;


// move cursor to final entity
def (warp)
{
  init ();

  auto e  = es[es.size()-1];
  g.cur.x = e->pos.x;
  g.cur.y = e->pos.y;

  g.keepCursorInFrame();
}

def (Delete)
{
  init ();

  auto e  = es[es.size()-1];
  g.cur.x = e->pos.x;
  g.cur.y = e->pos.y;

  for (auto e : es)
    g.world.remove (e);

  g.keepCursorInFrame();
}

def (Delete0)
{
  init ();

  for (auto e : es)
    g.world.remove (e);
}

def (_Delete)
{
  init ();

  auto e = es[es.size()-1];

  std::vector<Entity*> es2 {};
  subj (g.world, e->pos, es2);

  for (auto e2 : es2)
    if (e2 != e)
    {
      e = e2;
      break;
    }

  g.cur.x = e->pos.x;
  g.cur.y = e->pos.y;

  for (auto e : es)
    g.world.remove (e);

  g.keepCursorInFrame();
}

def (change)
{
  Delete0 (g, subj);
  g.km.setMode ('i');
}


#undef init
#undef def

}; // namespace Verb


#endif // MOTION_H
