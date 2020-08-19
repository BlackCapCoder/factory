#include "Undergroundee.h"
#include "Rendering.h"
#include <SDL2/SDL2_gfxPrimitives.h>


void Undergroundee::render (SDL_Renderer & rend, long time)
{
  // ----------- Background

  static constexpr float k = 0.14; // slope

  SDL_SetRenderDrawColor (&rend, 16*5, 16*5, 16*1, 255);

  SDL_FRect r = { 0, 0, 1, 1 };

  switch (!entrance ? dir : dswap (dir))
  {
    case (DIR::E):
      renderFillTrigon (rend, 1,   0, 0,   k, 1, k);
      renderFillTrigon (rend, 0, 1-k, 1, 1-k, 1, 1);
      r.x = 0; r.y = k; r.w = 1; r.h = 1-k*2;
      break;
    case (DIR::W):
      renderFillTrigon (rend, 0,0,1,k,0,k);
      renderFillTrigon (rend, 0,1-k,1,1-k,0,1);
      r.x = 0; r.y = k; r.w = 1; r.h = 1-k*2;
      break;
    case (DIR::N):
      renderFillTrigon (rend, 0,0,k,0,k,1);
      renderFillTrigon (rend, 1,0,1-k,0,1-k,1);
      r.x = k; r.y = 0; r.w = 1-k*2; r.h = 1;
      break;
    case (DIR::S):
      renderFillTrigon (rend, 0,1,k,1,k,0);
      renderFillTrigon (rend, 1,1,1-k,1,1-k,0);
      r.x = k; r.y = 0; r.w = 1-k*2; r.h = 1;
      break;
  }

  SDL_RenderFillRectF (&rend, &r);


  // ----------- Arrow

  static constexpr float bw = 0.2; // brush width
  static constexpr float bo = bw/2 * -1;
  static constexpr float bq = bw/4;

  // SDL_SetRenderDrawColor (&rend, 255, 255, 255, 255);
  SDL_SetRenderDrawColor     (&rend, 0, 0, 0, 255);
  SDL_SetRenderDrawBlendMode (&rend, SDL_BlendMode::SDL_BLENDMODE_MUL);

  if (entrance)
  {
    static constexpr float o = 0.4;
    static constexpr float l = 0.8;

    switch (dir)
    {
      case (DIR::W):
        renderDrawLine (rend, bw,  1-o,   k*o + bo, 1-l, 0.5 + bq);
        renderDrawLine (rend, bw,  1-o, 1-k*o - bo, 1-l, 0.5 - bq);
        break;
      case (DIR::E):
        renderDrawLine (rend, bw,    o,   k*o + bo,   l, 0.5 + bq);
        renderDrawLine (rend, bw,    o, 1-k*o - bo,   l, 0.5 - bq);
        break;
      case (DIR::N):
        renderDrawLine (rend, bw,    k*o + bo, 1-o, 0.5 + bq, 1-l);
        renderDrawLine (rend, bw,  1-k*o - bo, 1-o, 0.5 - bq, 1-l);
        break;
      case (DIR::S):
        renderDrawLine (rend, bw,    k*o + bo,   o, 0.5 + bq,   l);
        renderDrawLine (rend, bw,  1-k*o - bo,   o, 0.5 - bq,   l);
        break;
    }
  }
  else
  {
    static constexpr float o = 0.4;
    static constexpr float l = 0.7;

    switch (dir)
    {
      case (DIR::W):
        renderDrawLine (rend, bw,  1-o,   k*(1-o) + bo, 1-l, 0.5 + bq);
        renderDrawLine (rend, bw,  1-o, 1-k*(1-o) - bo, 1-l, 0.5 - bq);
        break;
      case (DIR::E):
        renderDrawLine (rend, bw,    o,   k*(1-o) + bo,   l, 0.5 + bq);
        renderDrawLine (rend, bw,    o, 1-k*(1-o) - bo,   l, 0.5 - bq);
        break;
      case (DIR::N):
        renderDrawLine (rend, bw,   k*(1-o) + bo, 1-o, 0.5 + bq, 1-l);
        renderDrawLine (rend, bw, 1-k*(1-o) - bo, 1-o, 0.5 - bq, 1-l);
        break;
      case (DIR::S):
        renderDrawLine (rend, bw,   k*(1-o) + bo,   o, 0.5 + bq,   l);
        renderDrawLine (rend, bw, 1-k*(1-o) - bo,   o, 0.5 - bq,   l);
        break;
    }
  }

  SDL_SetRenderDrawBlendMode (&rend, SDL_BlendMode::SDL_BLENDMODE_NONE);


  // ----------- Shading

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

