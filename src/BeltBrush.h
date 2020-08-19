#pragma once

#include "World.h"
#include "Belt.h"


class BeltBrush
{
  World & world;

public:

  V2 pos;

  constexpr BeltBrush (World & world, V2 pos)
    : world { world }
    , pos   { pos   }
  {
  }

  void move (DIR dout);

};

