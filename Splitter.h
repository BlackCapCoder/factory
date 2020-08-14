#ifndef SPLITTER_H
#define SPLITTER_H

#include <iostream>
#include <SDL2/SDL.h>
#include "Utils.h"
#include "Entity.h"
#include "World.h"
#include "Belt.h"


enum Priority
{
  Alternate = 0,
  Left      = 1,
  Right     = 2,
};


struct Splitter : public Entity
{
  DIR dout;
  static constexpr int cooldown = Belt::cooldown >> 1;
  int timeout = 0;

  Priority opri = Priority::Alternate;
  int buffer    = 0;
  bool state    = false;
  Item item;


  Splitter (int x, int y, DIR dout)
    : Entity
      ( V2 {x, y}
      , dout == DIR::N || dout == DIR::S ? V2 {2, 1} : V2 { 1, 2 }
      )
    , dout {dout}
  {}

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect r = { 0, 0, size.x, size.y };

    SDL_SetRenderDrawColor (&rend, 0, 0, 128, 255);
    SDL_RenderFillRect (&rend, &r);

    if (opri == Priority::Alternate)
      return;

    SDL_SetRenderDrawColor (&rend, 128, 128, 0, 255);

    const V2 p = opri == Priority::Left ? V2 {0,0} : dout == N || dout == S ? V2 {1,0} : V2 {0,1};

    SDL_FRect rf;

    const float w = 0.75;
    const float h = 0.15;
    const float o = 0.1;

    switch (dout)
    {
      case (DIR::N): rf = { p.x+(1-w)/2, p.y+o,       w, h }; break;
      case (DIR::S): rf = { p.x+(1-w)/2, p.y+(1-o*2), w, h }; break;
      case (DIR::W): rf = { p.x+o,       p.y+(1-w)/2, h, w }; break;
      case (DIR::E): rf = { p.x+(1-o*2), p.y+(1-w)/2, h, w }; break;
    }

    SDL_RenderFillRectF(&rend, &rf);
  }

  V2 pos2 ()
  {
    V2 r = pos;
    switch (dout)
    {
      case (DIR::N): r.x++; break;
      case (DIR::S): r.x++; break;
      case (DIR::W): r.y++; break;
      case (DIR::E): r.y++; break;
    }
    return r;
  }

  bool input (Item itm) override
  {
    if (buffer >= 2) return false;
    buffer++;
    item = itm;
    return true;
  }

  void tick (World & w, int dt) override
  {
    timeout -= dt;
    if (timeout > 0) return;

    if (buffer  == 0) return;

    V2 p1, p2;

    switch (dout)
    {
      case (DIR::N):
        p1.x = pos.x;
        p2.x = pos.x+1;
        p1.y = pos.y-1;
        p2.y = pos.y-1;
        break;
      case (DIR::S):
        p1.x = pos.x;
        p2.x = pos.x+1;
        p1.y = pos.y+1;
        p2.y = pos.y+1;
        break;
      case (DIR::W):
        p1.x = pos.x-1;
        p2.x = pos.x-1;
        p1.y = pos.y;
        p2.y = pos.y+1;
        break;
      case (DIR::E):
        p1.x = pos.x+1;
        p2.x = pos.x+1;
        p1.y = pos.y;
        p2.y = pos.y+1;
        break;
    }

    if ( (opri == Priority::Alternate && state)
      || (opri == Priority::Right)
       )
    {
      const auto tmp = p2;
      p2 = p1; p1 = tmp;
    }

    auto e = w.at (p1.x, p1.y);

    if (e != nullptr && e->input(item))
      { buffer--; state = !state; timeout = cooldown; }

    if (buffer == 0) return;

    e = w.at(p2.x, p2.y);

    if (opri == Priority::Alternate || buffer > 1)
    if (e != nullptr && e->input(item))
      {buffer--; state = !state; timeout = cooldown; }
  }
};


#endif // SPLITTER_H
