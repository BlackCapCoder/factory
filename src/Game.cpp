#include "Game.h"


void Game::render ()
{
  dirty = false;

  const float dx = ws.w/(u * cam.z);
  const float dy = ws.h/(u * cam.z);

  const auto cx = cam.x - dx/2;
  const auto cy = cam.y - dy/2;
  const auto sw = ws.w/(u*cam.z);
  const auto sh = ws.h/(u*cam.z);

  // Background
  {
    SDL_RenderSetScale    (&rend, 1, 1);
    SDL_RenderSetViewport (&rend, &ws);

    // SDL_SetRenderDrawColor (&rend, 0, 0, 0, 255);
    SDL_SetRenderDrawColor (&rend, 16, 16, 16, 255);
    SDL_RenderClear        (&rend);

    // World map
    {
      SDL_RenderSetScale(&rend, u * cam.z, u * cam.z);

      static constexpr float size = 0.75;

      SDL_FRect r = { 0, 0, size, size };

      const int ox = ceil (cx);
      const int oy = ceil (cy);

      int w = WorldMap::w;

      for (int y = 0; y < sh; y++)
        for (int x = 0; x < sw; x++)
          if (world.m.tile(x+ox,y+oy))
          {
            if (cam.z >= 0.25)
            {
              r.x = x + (1.0-size)/2; r.y = y + (1.0-size)/2;
              world.m.resource(x+ox, y+oy).render (rend, r);
            } else
            {
              Item itm = world.m.resource (x+ox, y+oy);
              itm.renderSetColor (rend, itm.quads[0].color);

              r.x = x; r.y = y; r.w = 1.0; r.h = 1.0;
              // SDL_RenderFillRectF (&rend, &r);
            }
          }

      if (cam.z < 0.25)
      {
        SDL_SetRenderDrawColor (&rend, 255, 255, 255, 255);

        const int u = world.m.rsize;
        static constexpr float size = 0.5;

        int offX = (int) ox % u; if (offX < 0) offX += u;
        int offY = (int) oy % u; if (offY < 0) offY += u;

        for (int y = 0; y < sh; y += u)
        {
          for (int x = 0; x < sw; x += u)
          {
            if (world.m.rtile(x+ox,y+oy))
            {
              r.x = x + u*(1-size)/2;
              r.y = y + u*(1-size)/2;
              r.w = u*size;
              r.h = u*size;

              r.x -= offX;
              r.y -= offY;

              world.m.resource(x+ox, y+oy).render (rend, r);
            }
          }
        }
      }

      SDL_RenderSetScale(&rend, 1, 1);
    }

    // Grid
    // if (cam.z >= 1.0)
    if (cam.z >= 0.75)
    {
      // SDL_SetRenderDrawColor (&rend, 255, 255, 255, 20);
      SDL_SetRenderDrawColor (&rend, 40, 40, 40, 255);

      for (float x = 0; x < ws.w; x += u*cam.z)
        SDL_RenderDrawLineF(&rend, x, 0, x, ws.h);
      for (float y = 0; y < ws.h; y += u*cam.z)
        SDL_RenderDrawLineF(&rend, 0, y, ws.w, y);
    }
  }

  SDL_RenderSetScale(&rend, u * cam.z, u * cam.z);

  world.render (rend, time, cx, cy, sw, sh);

  SDL_Rect r;

  // Cursor
  {
    r.x = cur.x - cx;
    r.y = cur.y - cy;
    r.w = 1;
    r.h = 1;

    SDL_Rect q { 0, 0, 1, 1 };

    // r.x -= cursorSize/2;
    // r.y -= cursorSize/2;

    r.w = cursorSize;
    r.h = cursorSize;

    q.w = cursorSize;
    q.h = cursorSize;

    SDL_RenderSetViewport(&rend, &r);

    SDL_SetRenderDrawBlendMode (&rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor     (&rend, 255, 255, 255, 40);
    SDL_RenderFillRect         (&rend, &q);
    SDL_SetRenderDrawBlendMode (&rend, SDL_BLENDMODE_NONE);
  }
}

