#ifndef UNDERGROUNDEE_H
#define UNDERGROUNDEE_H


#include <SDL2/SDL2_gfxPrimitives.h>
#include "Entity.h"
#include "World.h"
#include "Belt.h"


struct Undergroundee : Entity
{
  DIR  dir;
  bool entrance;
  bool cargo = false;
  Item item;
  Undergroundee * other = nullptr;

  // Maximum underground distance
  static constexpr int range = 4;

  Undergroundee (int x, int y, DIR dir, bool entrance)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dir   { dir }
    , entrance { entrance }
  {}

  void tick (World & w, int dt) override
  {
    if (entrance) return;
    if (!cargo) return;

    V2 p = pos + dir2V2 (dir);

    if (auto q = w.at(p.x, p.y); q != nullptr && q->input(item))
    {
      cargo = false;
      return;
    }
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_FRect r = { 0, 0, 1, 1 };

    // ------ Background
    static constexpr float k = 0.14; // slope

    SDL_SetRenderDrawColor (&rend, 64, 64, 16, 255);

    float sx, sy;
    SDL_RenderGetScale (&rend, &sx, &sy);
    SDL_RenderSetScale (&rend, 1.0, 1.0);

    switch (!entrance ? dir : dswap (dir))
    {
      case (DIR::E):
        filledTrigonRGBA (&rend, sx, 0,        0, k*sy,     sx, k*sy, 64, 64, 16, 255);
        filledTrigonRGBA (&rend, 0, sy*(1-k), sx, sy*(1-k), sx,   sy, 64, 64, 16, 255);
        r.x = 0; r.y = k; r.w = 1; r.h = 1-k*2;
        break;
      case (DIR::W):
        filledTrigonRGBA (&rend, 0,0,        sx,sy*k,     0,sy*k, 64, 64, 16, 255);
        filledTrigonRGBA (&rend, 0,sy*(1-k), sx,sy*(1-k), 0,sy,   64, 64, 16, 255);
        r.x = 0; r.y = k; r.w = 1; r.h = 1-k*2;
        break;
      case (DIR::N):
        filledTrigonRGBA (&rend, 0 , 0, k*sx,     0, k*sx,     sy, 64, 64, 16, 255);
        filledTrigonRGBA (&rend, sx, 0, sx*(1-k), 0, sx*(1-k), sy, 64, 64, 16, 255);
        r.x = k; r.y = 0; r.w = 1-k*2; r.h = 1;
        break;
      case (DIR::S):
        filledTrigonRGBA (&rend, 0,sy, k*sx,sy, k*sx,0, 64, 64, 16, 255);
        filledTrigonRGBA (&rend, sx,sy, sx*(1-k),sy, sx*(1-k),0, 64, 64, 16, 255);
        r.x = k; r.y = 0; r.w = 1-k*2; r.h = 1;
        break;
    }

    SDL_RenderSetScale  (&rend, sx, sy);
    SDL_RenderFillRectF (&rend, &r);


    // ------- Arrow

    static constexpr float o = 0.25;
    static constexpr float l = 0.75;
    static constexpr float w = 0.13;
    static constexpr unsigned char color = 16;

    SDL_RenderSetScale  (&rend, 1, 1);

    if (entrance)
    switch (dir)
    {
      case (DIR::W):
        thickLineRGBA (&rend, sx*(1-o),0,  sx*(1-l),sy/2, sx*w, color,color,color,255);
        thickLineRGBA (&rend, sx*(1-o),sy, sx*(1-l),sy/2, sx*w, color,color,color,255);
        break;
      case (DIR::E):
        thickLineRGBA (&rend, sx*o,0,  sx*l,sy/2, sx*w, color,color,color,255);
        thickLineRGBA (&rend, sx*o,sy, sx*l,sy/2, sx*w, color,color,color,255);
        break;
    }
    else
    switch (dir)
    {
      case (DIR::W):
        thickLineRGBA (&rend, sx*(1-o), sy*k*(1-o),     sx*(1-l), sy/2, sx*w, color,color,color,255);
        thickLineRGBA (&rend, sx*(1-o), sy*(1-k*(1-o)), sx*(1-l), sy/2, sx*w, color,color,color,255);
        break;
      case (DIR::E):
        thickLineRGBA (&rend, sx*o,sy*k*(1-o),     sx*l,sy/2, sx*w, color,color,color,255);
        thickLineRGBA (&rend, sx*o,sy*(1-k*(1-o)), sx*l,sy/2, sx*w, color,color,color,255);
        break;
    }


    SDL_RenderSetScale  (&rend, sx, sy);

    // ------ Shading
    static constexpr float s = 0.07;

    switch (!entrance ? dir : dswap (dir))
    {
      case (DIR::N):
        r.x = 0; r.y = 0; r.w = 1; r.h = s;
        break;
      case (DIR::S):
        r.x = 0; r.y = 1-s; r.w = 1; r.h = s;
        break;
      case (DIR::W):
        r.x = 0; r.y = 0; r.w = s; r.h = 1;
        break;
      case (DIR::E):
        r.x = 1-s; r.y = 0; r.w = s; r.h = 1;
        break;
    }

    SDL_SetRenderDrawColor (&rend, 0, 0, 0, 255);
    SDL_RenderFillRectF    (&rend, &r);

  }

  bool input (Item itm) override {
    if (entrance)
    {
      if (other == nullptr) return false;
      return other->input(itm);
    } else {
      if (cargo) return false;
      cargo = true;
      item  = itm;
      return true;
    }
  }
};

#endif // UNDERGROUNDEE_H
