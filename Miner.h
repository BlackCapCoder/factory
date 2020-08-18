#ifndef MINER_H
#define MINER_H

#include "Entity.h"
#include "Utils.h"
#include "World.h"
#include "Belt.h"
#include <SDL2/SDL2_gfxPrimitives.h>


struct Miner : Entity
{
  DIR  dout;
  int  timeout = 0;
  bool works  = false;
  Item itm;

  static constexpr int cooldown = Belt::cooldown * 5;
  // static constexpr int cooldown = Belt::cooldown * 1;

  Miner (int x, int y, DIR dout)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dout   { dout }
  {}

  void tick (World & w, int dt) override
  {
    works = w.m.tile(pos.x, pos.y);
    if (!works) return;
    itm = w.m.resource(pos.x, pos.y);

    timeout -= dt;
    if (timeout > 0) return;

    V2 p = pos + dir2V2(dout);

    if (auto q = w.at(p.x, p.y); q != nullptr && q->input(itm))
      timeout = cooldown;
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect r = { 0, 0, 1, 1 };

    {
      SDL_SetRenderDrawColor (&rend, 16*3, 16*3, 16*3, 255);

      unsigned char quads = 0;
      SDL_FRect rf;

      switch (dout)
      {
        case (DIR::S):
          quads = 0b11110000;
          rf    = SDL_FRect { 0.0, 0.5, 1.0, 0.5 };
          break;
        case (DIR::N):
          quads = 0b00001111;
          rf    = SDL_FRect { 0.0, 0.0, 1.0, 0.5 };
          break;
        case (DIR::E):
          quads = 0b00111100;
          rf    = SDL_FRect { 0.5, 0.0, 0.5, 1.0 };
          break;
        case (DIR::W):
          quads = 0b11000011;
          rf    = SDL_FRect { 0.0, 0.0, 0.5, 1.0 };
          break;
      }

      SDL_RenderFillRectF (&rend, &rf);

      float sx, sy;
      SDL_RenderGetScale (&rend, &sx, &sy);
      SDL_RenderSetScale (&rend, 1.0, 1.0);
      Item::fillCircle (&rend, 0.5*sx, 0.5*sy, sx*0.5, quads);
      SDL_RenderSetScale (&rend, sx, sy);
    }


    if (works)
    {
      static constexpr float off = 0.30;
      SDL_FRect rf { off, off, 1.0-off*2, 1.0-off*2 };
      itm.render (rend, rf);
    }
  }
  bool input (Item) override { return false; }
};


#endif // MINER_H
