#ifndef BELT_H
#define BELT_H

#include <SDL2/SDL.h>
#include "Utils.h"
#include "Entity.h"
#include "World.h"

class Belt : public Entity
{
public:
  DIR  din, dout;
  bool cargo     = false;
  int  timeout   = 0;

  static constexpr int cooldown = 300;

  Belt * pending = nullptr;

  Belt (int x, int y, DIR din, DIR dout)
    : Entity (V2 {x, y}, V2 {1, 1})
    , din    { din  }
    , dout   { dout }
  {}

  bool input () override
  {
    if (cargo) return false;
    cargo = true;
    timeout = cooldown;
    return true;
  }

  bool push (World & w)
  {
    if (!cargo)      return false;
    if (timeout > 0) return false;

    auto q = w.gr.m[this];

    if (q == nullptr) return false;

    if (q->input())
    {
      cargo = false;
      if (pending != nullptr)
      {
        pending->push (w);
        pending = nullptr;
      }
      return true;
    }
    else if (auto b = dynamic_cast<Belt*>(q))
    {
      b->pending = this;
    }

    return false;
  }

  void tick (World & w, int dt) override
  {
    timeout -= dt;
    if (timeout < 0) timeout = 0;
    push (w);
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect ur = { 0, 0, 1, 1 };
    SDL_SetRenderDrawColor (&rend, 16, 16, 16, 255);
    SDL_RenderFillRect (&rend, &ur);

    SDL_SetRenderDrawColor (&rend, 32, 32, 32, 255);

    static constexpr float bw = 0.1;

    SDL_FRect r;

    const bool N = din != DIR::N && dout != DIR::N;
    const bool E = din != DIR::E && dout != DIR::E;
    const bool S = din != DIR::S && dout != DIR::S;
    const bool W = din != DIR::W && dout != DIR::W;

    if (N)
    {
      r.x = 0.0;
      r.y = 0.0;
      r.w = 1.0;
      r.h = bw;
      SDL_RenderFillRectF (&rend, &r);
    }

    if (S)
    {
      r.x = 0.0;
      r.y = 1.0 - bw;
      r.w = 1.0;
      r.h = bw;
      SDL_RenderFillRectF (&rend, &r);
    }

    if (W)
    {
      r.x = 0.0;
      r.y = 0.0;
      r.w = bw;
      r.h = 1.0;
      SDL_RenderFillRectF (&rend, &r);
    }

    if (E)
    {
      r.x = 1.0 - bw;
      r.y = 0.0;
      r.w = bw;
      r.h = 1.0;
      SDL_RenderFillRectF (&rend, &r);
    }

    // --------------

    // SDL_SetRenderDrawColor (&rend, 46, 46, 0, 255);
    SDL_SetRenderDrawColor (&rend, 46, 46, 46, 255);

    static constexpr float w = 0.1;

    if (din == DIR::N || dout == DIR::N)
    {
      r.x = (1-w)/2;
      r.y = 0.0;
      r.w = w;
      r.h = 0.5;
      SDL_RenderFillRectF (&rend, &r);
    }
    if (din == DIR::S || dout == DIR::S)
    {
      r.x = (1-w)/2;
      r.y = 0.5;
      r.w = w;
      r.h = 0.5;
      SDL_RenderFillRectF (&rend, &r);
    }
    if (din == DIR::W || dout == DIR::W)
    {
      r.x = 0.0;
      r.y = (1-w)/2;
      r.w = 0.5;
      r.h = w;
      SDL_RenderFillRectF (&rend, &r);
    }
    if (din == DIR::E || dout == DIR::E)
    {
      r.x = 0.5;
      r.y = (1-w)/2;
      r.w = 0.5;
      r.h = w;
      SDL_RenderFillRectF (&rend, &r);
    }

    // SDL_SetRenderDrawColor (&rend, 0, 0, 0, 255);
  }

  void renderTrack (SDL_Renderer & rend, long time)
  {
    SDL_SetRenderDrawColor (&rend, 16, 16, 16, 255);
    renderCargoEx (rend, 1, true, 0.3, (float)(time%cooldown)/cooldown);
  }

  void renderCargoEx (SDL_Renderer & rend, float off, bool linear, float r, float a)
  {
    const float k = r;

    const float
      x1 = 0.5, y1 = k-r,
      x2 = 1.0 - k+r, y2 = 0.5,
      x3 = 0.5, y3 = 1.0 - k+r,
      x4 = k-r, y4 = 0.5;

    float px1, px2, py1, py2;

    switch (din)
    {
      case (DIR::N): px1 = x1; py1 = y1; break;
      case (DIR::E): px1 = x2; py1 = y2; break;
      case (DIR::S): px1 = x3; py1 = y3; break;
      case (DIR::W): px1 = x4; py1 = y4; break;
    }

    switch (dout)
    {
      case (DIR::N): px2 = x1; py2 = y1; break;
      case (DIR::E): px2 = x2; py2 = y2; break;
      case (DIR::S): px2 = x3; py2 = y3; break;
      case (DIR::W): px2 = x4; py2 = y4; break;
    }

    SDL_FRect R;


    if (!linear)
    {
      const float dx = px2 - px1;
      const float dy = py2 - py1;
      const float x  = px1 + dx * a - r/2;
      const float y  = py1 + dy * a - r/2;

      R = { off+x, off+y, r, r };
    }
    else if (a < 0.5)
    {
      a *= 2;
      const float dx = 0.5 - px1;
      const float dy = 0.5 - py1;
      const float x  = px1 + dx * a - r/2;
      const float y  = py1 + dy * a - r/2;

      R = { off+x, off+y, r, r };
    }
    else
    {
      a = a*2 - 1;
      const float dx = 0.5 - px2;
      const float dy = 0.5 - py2;
      const float x  = 0.5 - dx * a - r/2;
      const float y  = 0.5 - dy * a - r/2;

      R = { off+x, off+y, r, r };
    }

    SDL_RenderFillRectF(&rend, &R);
  }

  void renderCargo (SDL_Renderer & rend)
  {
    if (!cargo) return;
    const float a = (float) (cooldown - timeout) / (float) cooldown;
    SDL_SetRenderDrawColor (&rend, 128, 128, 128, 255);
    renderCargoEx (rend, 1, false, 0.3, a);
  }
};

#endif // BELT_H
