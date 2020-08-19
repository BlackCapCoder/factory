#include "BeltInsert.h"


void BeltInsert::placeBelt (int x, int y, DIR d1, DIR d2)
{
  if (d1 == d2)
    d1 = dswap (d2);

  if (auto q = g.world.at (x, y))
  {
    if (auto b = dynamic_cast<Belt*>(q))
    {
      b->din  = d1;
      b->dout = d2;
      g.world.gr.updateAround (q);
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

void BeltInsert::move (DIR d)
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

    g.cur += dir2V2(d);

    if (isUnder) return;

    DIR d2 = d;
    V2  p2 { g.cur.x, g.cur.y };

    if (!hasMoved)
    {
      bool N,E,S,W;

      if (auto q = g.world.at (p1.x-1, p1.y  ); q != nullptr) W = true; else W = false;
      if (auto q = g.world.at (p1.x+1, p1.y  ); q != nullptr) E = true; else E = false;
      if (auto q = g.world.at (p1.x  , p1.y-1); q != nullptr) N = true; else N = false;
      if (auto q = g.world.at (p1.x  , p1.y+1); q != nullptr) S = true; else S = false;

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

    if (auto q = dynamic_cast<Miner*>(g.world.at (p1.x, p1.y)))
    {
      q->dout = d1;
      g.world.gr.updateAround (q);
    }
    else
    if (auto q = g.world.bs.at (p1.x, p1.y))
    {
      q->dout = d2;
      if (q->din == d2) q->din = d1;
      g.world.gr.updateAround (q);
    }
    else
      placeBelt (p1.x, p1.y, dswap(d1), d2);


    if (auto q = g.world.bs.at (p2.x, p2.y))
    {
      q->din = dswap (d2);
      if (q->dout == q->din) q->dout = d2;
      g.world.gr.updateAround (q);
    }
    else
    {
      if (auto q = g.world.es.at (p2.x, p2.y))
      {
        // g.world.remove (q);
      }
      else
        placeBelt (p2.x, p2.y, dswap(d2), d2);
    }

    face     = d;
    hasMoved = true;
  }


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

  g.km.map('i', "y", [](){ BIst->NW (); });
  g.km.map('i', "u", [](){ BIst->NE (); });
  g.km.map('i', "b", [](){ BIst->SW (); });
  g.km.map('i', "n", [](){ BIst->SE (); });


  g.km.map('i', "m", [](){ BIst->placeMiner     (); });
  g.km.map('i', "s", [](){ BIst->placeSplitterR (); });
  g.km.map('i', "S", [](){ BIst->placeSplitterL (); });
  g.km.map('i', "t", [](){ BIst->placeTrash     (); });
  // g.km.map('i', "u", [](){ BIst->placeUndee     (); });
  g.km.map('i', "g", [](){ BIst->placeUndee     (); });

  g.km.map('i', "r", [](){ BIst->placeRotater   (); });


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

  g.km.map('n', "gi", [&g](){
    g.cur.x = BIst->initialPosition.x;
    g.cur.y = BIst->initialPosition.y;
    g.keepCursorInFrame();
  });

}

