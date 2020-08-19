#ifndef BELTINSERT_H
#define BELTINSERT_H

#include <vector>
#include "Game.h"
#include "Belt.h"
#include "Miner.h"
#include "Splitter.h"
#include "Trash.h"
#include "Undergroundee.h"
#include "Rotater.h"


struct BeltInsert
{
  Game & g;
  bool hasMoved = false;
  bool isUnder  = false;
  Undergroundee * u1 = nullptr;
  DIR udir;
  int udist = 0;

  V2  initialPosition {0,0};
  DIR face            {DIR::N};

  BeltInsert (Game & g) : g {g}
  {}

  void up    () {
    if (!reverse) move (DIR::N); else
    { g.cur.y--; move (DIR::S); g.cur.y--; }
    g.keepCursorInFrame ();
  }
  void down  () {
    if (!reverse) move (DIR::S); else
    { g.cur.y++; move (DIR::N); g.cur.y++; }
    g.keepCursorInFrame ();
  }
  void left  () {
    if (!reverse) move (DIR::W); else
    { g.cur.x--; move (DIR::E); g.cur.x--; }
    g.keepCursorInFrame ();
  }
  void right () {
    if (!reverse) move (DIR::E); else
    { g.cur.x++; move (DIR::W); g.cur.x++; }
    g.keepCursorInFrame ();
  }

  // ----------- Diagonal movement
  //
  // For each diagonal we have two options- for instance
  // to move north-west we can either move north then west,
  // or west then north.
  //
  // Rules:
  //   1) Choose the option that leaves 'face' unchanged if there is one.
  //   2) Choose the option for which the first move is NOT the opposite of 'face'

  // void NW () {
  //   switch (face)
  //   {
  //     case (DIR::N):
  //     case (DIR::S):
  //       left (); up (); break;
  //     case (DIR::W):
  //     case (DIR::E):
  //       up (); left (); break;
  //   }
  // }

  // void NE () {
  //   switch (face)
  //   {
  //     case (DIR::N):
  //     case (DIR::S):
  //       right (); up (); break;
  //     case (DIR::E):
  //     case (DIR::W):
  //       up (); right (); break;
  //   }
  // }
  //
  // void SW () {
  //   switch (face)
  //   {
  //     case (DIR::S):
  //     case (DIR::N):
  //       left (); down (); break;
  //     case (DIR::W):
  //     case (DIR::E):
  //       down (); left (); break;
  //   }
  // }
  //
  // void SE () {
  //   switch (face)
  //   {
  //     case (DIR::S):
  //     case (DIR::N):
  //       right (); down (); break;
  //     case (DIR::E):
  //     case (DIR::W):
  //       down (); right (); break;
  //   }
  // }


  // The scheme above have problems with corners.
  // Suppose the user is placing belts along a wall like this:
  //
  //    v|
  //    v|
  //    v|
  //    v+-------
  //
  // 'face' is south, so by rule 1 we move east first, then south,
  // which moves into the wall.
  //
  // Corners are quite common, so this is going to become annoying.
  //
  // ---- Scheme #2
  //
  // 1) If possible, the first move should be in direction 'face'.
  //    In this case 'face' should remain unchanged after both moves.
  //    (to make the behavior of holding a key down consistent)
  //
  //  2) Otherwise, choose the option that does begin with a move
  //     in the direction opposite to 'face'.

  void NW () {
    const DIR orig = face;
    if (reverse) face = dswap (face);

    switch (face)
    {
      case (DIR::N): up   (); left (); face = orig; break;
      case (DIR::E): up   (); left (); break;
      case (DIR::W): left (); up   (); face = orig; break;
      case (DIR::S): left (); up   (); break;
    }
  }
  void NE () {
    const DIR orig = face;
    if (reverse) face = dswap (face);

    switch (face)
    {
      case (DIR::N): up (); right (); face = orig; break;
      case (DIR::W): up (); right (); break;
      case (DIR::E): right (); up (); face = orig; break;
      case (DIR::S): right (); up (); break;
    }
  }
  void SW () {
    const DIR orig = face;
    if (reverse) face = dswap (face);

    switch (face)
    {
      case (DIR::S): down (); left (); face = orig; break;
      case (DIR::E): down (); left (); break;
      case (DIR::W): left (); down (); face = orig; break;
      case (DIR::N): left (); down (); break;
    }
  }
  void SE () {
    const DIR orig = face;
    if (reverse) face = dswap (face);

    switch (face)
    {
      case (DIR::S): down (); right (); face = orig; break;
      case (DIR::W): down (); right (); break;
      case (DIR::E): right (); down (); face = orig; break;
      case (DIR::N): right (); down (); break;
    }
  }


  // -----------

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

  void placeRotater ()
  {
    auto e = new Rotater { g.cur.x, g.cur.y, face };
    g.world.cleanAdd (e);
  }

  void placeBelt (int x, int y, DIR d1, DIR d2);
  void move (DIR d);

  bool reverse;
  void enter ()
  {
    if (auto q = g.world.at(g.cur.x, g.cur.y))
      if (auto b = dynamic_cast<Belt*>(q))
        face = b->dout;

    g.km.setMode('i');
    hasMoved = false;
    reverse  = false;
    initialPosition = V2 { g.cur.x, g.cur.y };
  }
  void enterRev ()
  {
    enter ();
    reverse = true;
  }
  void leave ()
  {
    g.km.setMode('n');
    isUnder = false;
    reverse = false;
  }
};

void beltInsert (Game & g);


#endif // BELTINSERT_H
