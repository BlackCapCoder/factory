#include "BeltBrush.h"


void BeltBrush::move (DIR dout)
{
  DIR din = dswap (dout);

  if (auto e = world.at (pos.x, pos.y))
  {
    if (auto b = dynamic_cast<Belt*>(e))
    {
      // din = b->dout;
      b->dout = dout;
      if (b->din == dout) b->din = din;
      // g.world.gr.updateAround (e);
    }

  }
  else
  {
    world.add (new Belt { pos.x, pos.y, din, dout });
    // placeBelt (p1.x, p1.y, dswap(d1), d2);
  }

  // --------

  const V2 newPos
    = pos + dir2V2 (dout);

  if (auto e = world.at (newPos))
  {
    if (auto b = dynamic_cast<Belt*>(e))
    {
      b->din = dswap (dout);
      if (b->dout == b->din) b->dout = dout;
      // g.world.gr.updateAround (q);
    }
  } else
  {
    world.add (new Belt { newPos.x, newPos.y, din, dout });
    // placeBelt (p2.x, p2.y, dswap(d2), d2);
  }

  pos = newPos;
}

