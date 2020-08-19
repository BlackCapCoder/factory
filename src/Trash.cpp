#include "Trash.h"


void Trash::render (SDL_Renderer & rend, long time)
{
  SDL_Rect r = { 0, 0, 1, 1 };

  SDL_SetRenderDrawColor (&rend, 128, 0, 0, 255);
  SDL_RenderFillRect     (&rend, &r);
}

