#ifndef MINER_H
#define MINER_H

#include "Entity.h"
#include "Utils.h"
#include "World.h"
#include "Belt.h"


struct Miner : Entity
{
  DIR dout;
  int timeout = 0;
  bool works  = false;

  static constexpr int cooldown = Belt::cooldown * 5;
  // static constexpr int cooldown = Belt::cooldown * 1;

  Miner (int x, int y, DIR dout)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dout   { dout }
  {}

  void tick (World & w, int dt) override
  {
    works = w.m.tile(pos.x, pos.y);
    // if (!works) return;

    timeout -= dt;
    if (timeout > 0) return;

    V2 p = pos + dir2V2(dout);

    Item itm { Gray };

    if (auto q = w.at(p.x, p.y); q != nullptr && q->input(itm))
      timeout = cooldown;
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect r = { 0, 0, 1, 1 };

    if (works)
      SDL_SetRenderDrawColor (&rend, 0, 128, 0, 255);
    else
      SDL_SetRenderDrawColor (&rend, 0, 40, 0, 255);

    SDL_RenderFillRect (&rend, &r);
  }
  bool input (Item) override { return false; }
};


#endif // MINER_H
