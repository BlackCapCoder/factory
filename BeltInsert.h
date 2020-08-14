#ifndef BELTINSERT_H
#define BELTINSERT_H

#include <vector>
#include "Game.h"
#include "Belt.h"
#include "Miner.h"
#include "Splitter.h"
#include "Trash.h"
#include "Undergroundee.h"


struct BeltInsert
{
  Game & g;
  DIR face;
  bool hasMoved  = false;
  bool isUnder = false;
  Undergroundee * u1 = nullptr;
  DIR udir;
  int udist = 0;

  BeltInsert (Game & g) : g {g}
  {}

  void up    () {
    if (!reverse) move (DIR::N); else
    { g.cur.y--; move (DIR::S); g.cur.y--; }
  }
  void down  () {
    if (!reverse) move (DIR::S); else
    { g.cur.y++; move (DIR::N); g.cur.y++; }
  }
  void left  () {
    if (!reverse) move (DIR::W); else
    { g.cur.x--; move (DIR::E); g.cur.x--; }
  }
  void right () {
    if (!reverse) move (DIR::E); else
    { g.cur.x++; move (DIR::W); g.cur.x++; }
  }

  void placeMiner ()
  {
    if (isUnder) return;
    g.world.cleanAdd(new Miner {g.cur.x, g.cur.y, face});
  }
  void placeTrash ()
  {
    if (isUnder) return;
    g.world.cleanAdd(new Trash {g.cur.x, g.cur.y});
  }

  // right-hand rule: splitter goes on the right-hand side of the belt
  //
  //  SS  >>>>>>S   v
  //  |         S   v
  //  |   S         v
  //  |   S<<<<<<  SS
  //
  void placeSplitterR (DIR face)
  {
    if (isUnder) return;

    auto x = g.cur.x;
    auto y = g.cur.y;

    switch (face)
    {
      case (DIR::W): y--; break;
      case (DIR::S): x--; break;
    }

    auto s = new Splitter {x, y, face};
    g.world.cleanAdd(s);
  }
  void placeSplitterR () { placeSplitterR (face); }

  // left-hand rule
  void placeSplitterL (DIR face)
  {
    if (isUnder) return;

    auto x = g.cur.x;
    auto y = g.cur.y;

    switch (face)
    {
      case (DIR::E): y--; break;
      case (DIR::N): x--; break;
    }

    auto s = new Splitter {x, y, face};
    g.world.cleanAdd(s);
  }
  void placeSplitterL () { placeSplitterL (face); }


  void placeUndee ()
  {
    auto u2 = new Undergroundee {g.cur.x, g.cur.y, face, !isUnder};

    if (isUnder)
    {
      if (udist == 0) return;

      u2->other = u1;
      u1->other = u2;
    } else {
      u1   = u2;
      udir = face;
      udist = 0;
    }

    isUnder = !isUnder;
    g.world.cleanAdd(u2);
  }

  void placeBelt (int x, int y, DIR d1, DIR d2)
  {
    if (d1 == d2)
      d1 = dswap (d2);

    if (auto q = g.world.at (x, y); q != nullptr)
    {
      if (auto b = dynamic_cast<Belt*>(q); b != nullptr)
      {
        b->din  = d1;
        b->dout = d2;
      }
      else
      {
        return;
      }
    }
    else
    {
      g.world.add (new Belt { x, y, d1, d2 });
    }
  }

  void move (DIR d)
  {
    DIR d1 = face;
    V2  p1 { g.cur.x, g.cur.y };

    if (isUnder)
    {
      if (d != udir && dswap(d) != udir)
        return;

      if (d == udir)
      {
        if (udist-1 == Undergroundee::range)
        {
          placeUndee();
        } else udist++;
      } else
      {
        if (udist == 0) return;
        udist--;
      }

    }

    switch (d)
    {
      case (DIR::N): g.cur.y--; g.keepCursorInFrame (); break;
      case (DIR::S): g.cur.y++; g.keepCursorInFrame (); break;
      case (DIR::W): g.cur.x--; g.keepCursorInFrame (); break;
      case (DIR::E): g.cur.x++; g.keepCursorInFrame (); break;
    }


    if (isUnder) return;

    DIR d2 = d;
    V2  p2 { g.cur.x, g.cur.y };

    if (!hasMoved)
    {
      bool N,E,S,W;

      if (auto q = g.world.es.at (p1.x-1, p1.y  ); q != nullptr) W = dynamic_cast<Belt*>(q) == nullptr; else W = false;
      if (auto q = g.world.es.at (p1.x+1, p1.y  ); q != nullptr) E = dynamic_cast<Belt*>(q) == nullptr; else E = false;
      if (auto q = g.world.es.at (p1.x  , p1.y-1); q != nullptr) N = dynamic_cast<Belt*>(q) == nullptr; else N = false;
      if (auto q = g.world.es.at (p1.x  , p1.y+1); q != nullptr) S = dynamic_cast<Belt*>(q) == nullptr; else S = false;

      bool done = false;

      switch (d)
      {
        case (DIR::N): if (S) {d1 = d; done = true;} break;
        case (DIR::S): if (N) {d1 = d; done = true;} break;
        case (DIR::W): if (E) {d1 = d; done = true;} break;
        case (DIR::E): if (W) {d1 = d; done = true;} break;
      }

      if (!done)
      {
        if      (W) d1 = DIR::E;
        else if (E) d1 = DIR::W;
        else if (N) d1 = DIR::S;
        else if (S) d1 = DIR::N;
        else        d1 = d;
      }
    }

    if (auto q = dynamic_cast<Miner*>(g.world.es.at (p1.x, p1.y)))
      q->dout = d;
    else
    if (auto q = g.world.bs.at (p1.x, p1.y))
    {
      q->dout = d;
    }
    else
      placeBelt (p1.x, p1.y, dswap(d1), d2);


    if (auto q = g.world.bs.at (p2.x, p2.y))
      q->din = dswap (d2);
    else
    {
      if (auto q = g.world.es.at (p2.x, p2.y))
        {} // g.world.remove (q);
      else
        placeBelt (p2.x, p2.y, dswap(d2), d2);
    }

    face     = d;
    hasMoved = true;
  }

  bool reverse;
  void enter ()
  {
    g.km.setMode('i');
    hasMoved = false;
    reverse  = false;
  }
  void enterRev ()
  {
    g.km.setMode('i');
    hasMoved = false;
    reverse  = true;
  }
  void leave ()
  {
    g.km.setMode('n');
    isUnder = false;
    reverse = false;
  }
};
BeltInsert * BIst;

void beltInsert (Game & g)
{
  BIst = new BeltInsert { g };

  g.km.map('n', "i",     [](){ BIst->enter (); });
  g.km.map('n', "I",     [](){ BIst->enterRev (); });
  g.km.map('i', "<esc>", [](){ BIst->leave (); });

  g.km.map('i', "h", [](){ BIst->left  (); });
  g.km.map('i', "l", [](){ BIst->right (); });
  g.km.map('i', "k", [](){ BIst->up    (); });
  g.km.map('i', "j", [](){ BIst->down  (); });

  g.km.map('i', "m", [](){ BIst->placeMiner     (); });
  g.km.map('i', "s", [](){ BIst->placeSplitterR (); });
  g.km.map('i', "S", [](){ BIst->placeSplitterL (); });
  g.km.map('i', "t", [](){ BIst->placeTrash     (); });
  g.km.map('i', "u", [](){ BIst->placeUndee     (); });

  // ----

  g.km.map('n', "s",     [&g](){ g.km.setMode('s', 'n'); });
  g.km.map('n', "S",     [&g](){ g.km.setMode('S', 'n'); });

  g.km.map('s', "h", [&g](){ BIst->placeSplitterR(DIR::W); g.km.setMode('n'); });
  g.km.map('s', "j", [&g](){ BIst->placeSplitterR(DIR::S); g.km.setMode('n'); });
  g.km.map('s', "k", [&g](){ BIst->placeSplitterR(DIR::N); g.km.setMode('n'); });
  g.km.map('s', "l", [&g](){ BIst->placeSplitterR(DIR::E); g.km.setMode('n'); });

  g.km.map('S', "h", [&g](){ BIst->placeSplitterL(DIR::W); g.km.setMode('n'); });
  g.km.map('S', "j", [&g](){ BIst->placeSplitterL(DIR::S); g.km.setMode('n'); });
  g.km.map('S', "k", [&g](){ BIst->placeSplitterL(DIR::N); g.km.setMode('n'); });
  g.km.map('S', "l", [&g](){ BIst->placeSplitterL(DIR::E); g.km.setMode('n'); });

}


#endif // BELTINSERT_H
