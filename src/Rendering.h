#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL2_gfxPrimitives.h>


void renderFillTrigon
  ( SDL_Renderer & rend
  , float x1, float y1
  , float x2, float y2
  , float x3, float y3
  );

void renderDrawLine
  ( SDL_Renderer & rend
  , float brushWidth
  , float x1, float y1
  , float x2, float y2
  );


#endif // RENDERING_H
