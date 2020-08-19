#include "Rotater.h"


void Rotater::render (SDL_Renderer & rend, long time)
{
  SDL_FRect r = { 0, 0, 1, 1 };

  SDL_SetRenderDrawColor (&rend, 64, 128, 128, 255);
  SDL_RenderFillRectF (&rend, &r);

  static constexpr float s = 0.2;
  static constexpr float o = 0.1;

  switch (dout)
  {
    case (DIR::N):
      r.x = (1-s)/2; r.y = o; r.w = s; r.h = s;
      break;
    case (DIR::S):
      r.x = (1-s)/2; r.y = 1-o-s; r.w = s; r.h = s;
      break;
    case (DIR::W):
      r.y = (1-s)/2; r.x = o;  r.w = s; r.h = s;
      break;
    case (DIR::E):
      r.y = (1-s)/2; r.x = 1-o-s; r.w = s; r.h = s;
      break;
  }

  SDL_SetRenderDrawColor (&rend, 255, 0, 0, 255);
  SDL_RenderFillRectF    (&rend, &r);
}

