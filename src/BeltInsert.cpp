#include "BeltInsert.h"
#include "Rotater.h"


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

void BeltInsert::undeeMove (DIR d)
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

  g.cur += dir2V2(d);
}

// ----------


void BeltInsert::move (DIR d)
{
  if (isBlock)
  {
    blockMove (d, blockSize);
    return;
  }

  if (isUnder)
  {
    undeeMove (d);
    return;
  }

  // -------

  DIR d1 = face;
  DIR d2 = d;

  V2 p1 { g.cur.x, g.cur.y };
  V2 p2 = p1 + dir2V2 (d);

  g.cur = p2;

  // Try to connect to neighboring entities
  if (!hasMoved)
  {
    bool N{false},E{false},S{false},W{false};

    if (auto q = g.world.at (p1.x-1, p1.y  ))
    if (auto b = dynamic_cast<Belt*>(q))
      W = b->dout == DIR::E;
    else
      W = true;

    if (auto q = g.world.at (p1.x+1, p1.y  ))
    if (auto b = dynamic_cast<Belt*>(q))
      E = b->dout == DIR::W;
    else
      E = true;

    if (auto q = g.world.at (p1.x  , p1.y-1))
    if (auto b = dynamic_cast<Belt*>(q))
      N = b->dout == DIR::S;
    else
      N = true;

    if (auto q = g.world.at (p1.x  , p1.y+1))
    if (auto b = dynamic_cast<Belt*>(q))
      S = b->dout == DIR::N;
    else
      S = true;

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

  // Rotate an entity's output as we move away from it
  //
  if (auto q = g.world.at (p1))
  {
    if (auto e = dynamic_cast<Miner*>(q))
    {
      e->dout = d;
      g.world.gr.updateAround (e);
    }
    else
    if (auto e = dynamic_cast<Rotater*>(q))
    {
      e->dout = d;
      g.world.gr.updateAround (e);
    }
    else
    if (auto e = dynamic_cast<Belt*>(q))
    {
      e->dout = d2;
      if (e->din == d2) e->din = d1;
      g.world.gr.updateAround (e);
    }
    else
    {
      placeBelt (p1.x, p1.y, dswap(d1), d2);
    }
  }
  else
  {
    placeBelt (p1.x, p1.y, dswap(d1), d2);
  }

  // If we move onto a belt, rotate its input towards us
  if (auto q = g.world.bs.at (p2.x, p2.y))
  {
    q->din = dswap (d2);
    if (q->dout == q->din) q->dout = d2;
    g.world.gr.updateAround (q);
  }
  else
  {
    placeBelt (p2.x, p2.y, dswap(d2), d2);
  }

  face     = d;
  hasMoved = true;
}


void BeltInsert::blockMove (DIR d, const int n)
{
  DIR d1 = face;
  DIR d2 = d;

  V2 pos { g.cur.x, g.cur.y };

  g.cur   += dir2V2 (d);
  hasMoved = true;
  face     = d;

  if (auto e = g.world.at (pos))
  {
    if (auto b = dynamic_cast<Belt*>(e))
    {
      d1 = b->dout;
    }
    else
    {
      d1 = d2;
    }
  }
  else
  {
    d1 = d2;
  }

  // --------


  if (d1 == dswap (d2)) // opposite
  {
    return; // TODO
  }
  else if (d1 == d2)  // straight
  {
    const int k = n/2;

    for (int i = 0; i < n; ++i)
    {
      V2 p = pos;

      if (d2 == DIR::E || d2 == DIR::W )
      {
        p.y += i-k;
        if (d2 == DIR::E) p.x += k; else p.x -= k;
      }
      else
      {
        p.x += i-k;
        if (d2 == DIR::S) p.y += k; else p.y -= k;
      }


      BeltBrush bb
        { g.world, p };

      bb.move (d2);
    }


    return;
  }
  else // diagonal
  {

    // false  | true
    //        |
    //     /  |  \
    //    /   |   \
    //   /    |    \
    //        |
    // 1  ^<  |  ^>  1
    // 1  >v  |  >^  0
    // 0  v>  |  v<  0
    // 0  <^  |  <v  1
    bool diag;

    switch (d1)
    {
      case DIR::N:   diag = d2 == DIR::E;   break;
      case DIR::E:   diag = d2 == DIR::N;   break;
      case DIR::S:   diag = d2 == DIR::W;   break;
      case DIR::W:   diag = d2 == DIR::S;   break;
    }

    // high y-coord first?
    bool vertical
      =  d1 == DIR::N
      || d2 == DIR::S
      ;

    // ----

    const int k = n/2;

    for (int i = 0; i < n; ++i)
    {
      const V2 p =
        pos + V2 { diag ? i - k : k - i, i - k };

      const int x =
        vertical ? n - 1 - i : i;

      BeltBrush bb
        { g.world , p };

      for (int j = 0; j <= x; ++j)
        bb.move (d2);
    }

    return;
  }

}


void BeltInsert::placeBeltEx (int x, int y, DIR face, DIR d)
{
  const V2 p1 { x, y };
  const V2 p2 = p1 + dir2V2 (d);

  placeBelt (p1.x, p1.y, dswap (face), d);
  // g.world.add (new Belt { p1.x, p1.y, face, d });

  // placeBelt (p2.x, p2.y, dswap(d), d);
}


// -------------------------

BeltInsert * BIst;

void beltInsert (Game & g)
{
  BIst = new BeltInsert { g };

  g.km.map('n', "i",     [](){ BIst->enter (); });
  g.km.map('n', "I",     [](){ BIst->enterRev (); });
  g.km.map('n', "<c-i>", [](){ BIst->enterBlock (); });
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

  g.km.map('i', "+", [](){ BIst->incBlock(); });
  g.km.map('i', "-", [](){ BIst->decBlock(); });

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

