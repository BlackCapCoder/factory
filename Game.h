#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL_opengl.h>

#include "Utils.h"
#include "Key.h"
#include "KeyMap.h"
#include "Utils.h"
#include "Belt.h"
#include "Entity.h"
#include "Base.h"


class Game
{
  SDL_Window   & wndw;
  SDL_Renderer & rend;

  static constexpr int resolution = 1080;

public:
  Game () :
  wndw { *SDL_CreateWindow
    ( "factory"
    , SDL_WINDOWPOS_CENTERED
    , SDL_WINDOWPOS_CENTERED
    , resolution*16/9
    , resolution
    , SDL_WINDOW_SHOWN
    | SDL_WINDOW_OPENGL
    | SDL_WINDOW_RESIZABLE
    ) },
  rend
    { *SDL_CreateRenderer
      ( &wndw
      , -1
      , SDL_RENDERER_ACCELERATED
      | SDL_RENDERER_PRESENTVSYNC
      ) }
  {
    SDL_SetRenderDrawBlendMode(&rend, SDL_BLENDMODE_BLEND);
  }

public:
  Camera     cam { 0, 0, 1 };
  V2         cur { 0, 0 };
  World      world {};

  SDL_Rect ws {};
  bool quit  = false;
  bool dirty = false;

  const float u  = 80;

  void transform (float a, float b, float c, float d, float e, float f)
  {
    float mat[16]
      { a, c, e, 0
      , b, d, f, 0
      , 0, 0, 1, 0
      , 0, 0, 0, 1
      };

    glLoadMatrixf (mat);
  }

  void render ()
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

      SDL_SetRenderDrawColor (&rend, 0, 0, 0, 255);
      SDL_RenderClear        (&rend);

      // World map
      {
        SDL_RenderSetScale(&rend, u * cam.z, u * cam.z);

        SDL_FRect r = { 0, 0, 1, 1 };
        SDL_SetRenderDrawColor (&rend, 32, 16, 32, 255);

        const int ox = ceil (cx);
        const int oy = ceil (cy);

        int w = WorldMap::w;

        for (int y = 0; y < sh; y++)
          for (int x = 0; x < sw; x++)
            if (world.m.tile(x+ox,y+oy))
            {
              r.x = x; r.y = y;
              world.m.resource(x+ox, y+oy).render (rend, r);
            }

        SDL_RenderSetScale(&rend, 1, 1);
      }

      // Grid
      if (cam.z > 1.1)
      {
        SDL_SetRenderDrawColor (&rend, 255, 255, 255, 20);

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

      SDL_RenderSetViewport(&rend, &r);

      SDL_SetRenderDrawColor (&rend, 255, 255, 255, 40);
      SDL_RenderFillRect     (&rend, &q);
    }
  }

  void tick (int dt)
  {
    world.tick (dt);
  }

  long time = 0;
  void loop ()
  {
    auto lastTick = std::chrono::system_clock::now ();

    using Dur = std::chrono::duration<double, std::milli>;

    do
    {
      const auto t  = std::chrono::system_clock::now ();
      const auto dt = Dur{t - lastTick}.count();

      dirty    = true;
      lastTick = t;
      time    += dt;

      handleInput ();
      tick        (dt);

      if (dirty || true)
      {
        render ();
        SDL_RenderPresent (&rend);
      }

      // SDL_Delay(1000/60 - dt);

      std::this_thread::sleep_for (Dur {1000/60 - dt});
      // std::this_thread::sleep_for (Dur {1000/1 - dt});

    } while (!quit);
  }

  // Move the camera such that the given point is at least
  // 'off' tiles away from all screen edges
  void keepInFrame (int px, int py)
  {
    const auto off = _scrolloff;
    const auto _w = ws.w/(u*cam.z);
    const auto _h = ws.h/(u*cam.z);
    const auto dx = _w/2;
    const auto dy = _h/2;
    const auto x  = cam.x - dx + 0.5;
    const auto y  = cam.y - dy + 0.5;
    const auto ow = dx >= off ? off : dx;
    const auto oh = dy >= off ? off : dy;

    if (px < x + ow) // E
      cam.x = px + dx - ow;
    else if (px >= x + _w - ow - 1) // W
      cam.x = px - dx + ow;

    if (py < y + oh) // N
      cam.y = py + dy - oh;
    else if (py >= y + _h - oh - 1) // S
      cam.y = py - dy + oh;
  }

  // Move the camera such that the given point is exactly 'off'
  // tiles away from the 'dir' screen edge
  void scrolloff (int px, int py, DIR dir)
  {
    const auto off = _scrolloff;
    const auto _w = ws.w/(u*cam.z);
    const auto _h = ws.h/(u*cam.z);
    const auto dx = _w/2;
    const auto dy = _h/2;
    const auto x  = cam.x - dx + 0.5;
    const auto y  = cam.y - dy + 0.5;
    const auto ow = dx >= off ? off : dx;
    const auto oh = dy >= off ? off : dy;

    switch (dir)
    {
      case (DIR::W): cam.x = px + dx - ow; break;
      case (DIR::N): cam.y = py + dy - oh; break;
      case (DIR::E): cam.x = px - dx + ow + 1; break;
      case (DIR::S): cam.y = py - dy + oh + 1; break;
    }
  }

  // Move the camera to keep the cursor in frame
  void keepCursorInFrame ()
  {
    keepInFrame(cur.x, cur.y);
    std::cout << cam.x << ", " << cam.y << std::endl;
  }

  // Move the cursor, bringing it into frame
  void moveCursorIntoFrame ()
  {
    const auto off = _scrolloff;
    const auto _w = ws.w/(u*cam.z);
    const auto _h = ws.h/(u*cam.z);
    const auto dx = _w/2;
    const auto dy = _h/2;
    const auto x  = cam.x - dx + 0.5;
    const auto y  = cam.y - dy + 0.5;
    const auto ow = dx >= off ? off : dx;
    const auto oh = dy >= off ? off : dy;

    const auto px = cur.x;
    const auto py = cur.y;

    if (px < x + ow)
      cur.x = x + ow;
    else
    if (px >= x + _w - ow - 1)
      cur.x = x + _w - ow;

    if (py < y + oh)
      cur.y = y + oh;
    else
    if (py >= y + _h - oh - 1)
      cur.y = y + _h - oh;
  }

public:

  KeySTM km {};

  bool is_x_held = false;

  // int _scrolloff = 4;
  int _scrolloff = 16;
  // int _scrolloff = 100000;


public:

  static constexpr int   ZOOM     = 4;
  static constexpr float ZOOM_MIN = 0.1;
  static constexpr float ZOOM_MAX = 2.0;

  int zoom = 0;

  void zoomIn ()
  {
    if (cam.z >= ZOOM_MAX) return;
    zoom++;
    updateZoom ();
  }
  void zoomOut ()
  {
    if (cam.z <= ZOOM_MIN) return;
    zoom--;
    updateZoom ();
  }
  void zoomEnd ()
  {
    zoom = ZOOM * -1;
    updateZoom ();
  }
  void zoomReset ()
  {
    zoom = 0;
    updateZoom ();
  }

private:
  void updateZoom ()
  {
    cam.z = 1.0 + (1.0 / ZOOM) * zoom;

    if (cam.z > ZOOM_MAX)
      cam.z = ZOOM_MAX; else
    if (cam.z < ZOOM_MIN)
      cam.z = ZOOM_MIN;

    keepCursorInFrame ();
  }

private:

  void handleInput ()
  {
    auto ev = SDL_Event{};

    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_WINDOWEVENT)
      {
        if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          ws.w = ev.window.data1;
          ws.h = ev.window.data2;
          dirty = true;
        }
        if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
          dirty = true;
        }
        continue;
      }

      if (ev.type == SDL_KEYDOWN)
      {
        if (ev.key.keysym.sym == 'x' && ev.key.keysym.mod == KMOD_NONE)
          is_x_held = true;
      }
      if (ev.type == SDL_KEYUP)
      {
        if (ev.key.keysym.sym == 'x' && ev.key.keysym.mod == KMOD_NONE)
          is_x_held = false;
      }

      if (ev.type == SDL_KEYDOWN)
      {
        if (isKeyComposible (ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod))
          continue;

        Key k { ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod };

        if (isKeyComposible (ev.key.keysym.sym, KMOD_NONE))
          k.mod = (SDL_Keymod) (k.mod | Key::MOD_CHAR);

        km.put(k);
        dirty = true;
      }
      if (ev.type == SDL_TEXTINPUT)
      {
        SDL_Keymod mod = (SDL_Keymod) (SDL_GetModState() | Key::MOD_CHAR);
        mod = (SDL_Keymod) (mod xor (mod & KMOD_SHIFT));

        Key k { (char) ev.text.text[0], mod };
        km.put(k);
        dirty = true;
      }
    }
  }

};


#endif // GAME_H
