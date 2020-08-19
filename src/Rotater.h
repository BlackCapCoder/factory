#ifndef ROTATER_H
#define ROTATER_H

#include "Entity.h"
#include "World.h"
#include "Belt.h"


struct Rotater : public Entity
{
  static constexpr int cooldown = Belt::cooldown*8;
  int timeout = 0;

  DIR dout;
  bool cw      = true;
  bool hasItem = false;
  Item item;

  Rotater (int x, int y, DIR dout)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dout { dout }
  {}

  void tick (World & w, int dt) override
  {
    if (!hasItem) return;
    timeout -= dt;
    if (timeout > 0) return;

    if (auto e = w.at(pos + dir2V2 (dout)))
    {
      if (e->input (item))
        hasItem = false;
    }
  }

  void render (SDL_Renderer & rend, long time) override;

  bool input (Item i) override
  {
    if (hasItem) return false;
    item    = Item { i.quads[3], i.quads[0], i.quads[1], i.quads[2] };
    hasItem = true;
    timeout = cooldown;
    return true;
  }
};

#endif // ROTATER_H
