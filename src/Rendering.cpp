#include "Rendering.h"


void renderFillTrigon
  ( SDL_Renderer & rend
  , float x1, float y1
  , float x2, float y2
  , float x3, float y3 )
{
  float sx, sy;
  Uint8 r,g,b,a;

  SDL_RenderGetScale     (&rend, &sx, &sy);
  SDL_GetRenderDrawColor (&rend, &r, &g, &b, &a);

  // ----

  SDL_RenderSetScale (&rend, 1.0, 1.0);

  filledTrigonRGBA
    ( &rend
    , x1 * sx, y1 * sy
    , x2 * sx, y2 * sy
    , x3 * sx, y3 * sy
    , r, g, b, a
    );

  SDL_RenderSetScale  (&rend, sx, sy);
}

void renderDrawLine
  ( SDL_Renderer & rend
  , float brushWidth
  , float x1, float y1
  , float x2, float y2 )
{
  float sx, sy;
  Uint8 r,g,b,a;

  SDL_RenderGetScale     (&rend, &sx, &sy);
  SDL_GetRenderDrawColor (&rend, &r, &g, &b, &a);

  // ----

  const float bw = brushWidth * sx;
  const float bo = bw / 2;


  SDL_RenderSetScale (&rend, 1.0, 1.0);

  thickLineRGBA
    ( &rend
    , x1 * sx, y1 * sy
    , x2 * sx, y2 * sy
    , bw
    , r, g, b, a
    );

  SDL_RenderSetScale  (&rend, sx, sy);
}

