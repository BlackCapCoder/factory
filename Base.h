#ifndef BASE_H
#define BASE_H

#include "Utils.h"
#include "Entity.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>

struct Base : Entity
{
  Base (int x, int y)
    : Entity (V2 {x, y}, V2 {4, 4})
  {
    loadFont ();
  }

  void tick (World & w, int dt) override
  {
    // auto e = w.bs.at(pos.x, pos.y);
    // if (e == nullptr) return;
    // e->cargo = false;
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect r = { 0, 0, size.x, size.y };
    SDL_SetRenderDrawColor (&rend, 90, 90, 90, 255);
    SDL_RenderFillRect (&rend, &r);

    static constexpr float s = 0.25;

    // Request display
    {
      SDL_FRect rf { (1-s)/2*size.x, (1-s)*0.4*size.y, s*size.x, s*size.y };

      SDL_SetRenderDrawColor (&rend, 192, 192, 192, 255);
      SDL_RenderFillRectF (&rend, &rf);
    }

    float sx, sy;
    SDL_RenderGetScale(&rend, &sx, &sy);
    SDL_RenderSetScale(&rend, 1, 1);

    // Text
    if (!hasText)
    {
      char str[32];
      std::sprintf(str, "%u/%u", delivered, 1000);
      setText (rend, str);
      hasText = true;
    }

    {
      static constexpr float padding = 0.15;
      float z = sx/tileUnit;

      float w = surface->w;
      float h = surface->h;
      float x = (4*tileUnit-w)/2;
      float y = (4*tileUnit*(0.4+s/2+padding));
      SDL_FRect rf {x, y, w, h};

      SDL_RenderSetScale(&rend, z, z);
      SDL_RenderCopyF (&rend, texture, nullptr, &rf);
    }
    SDL_RenderSetScale(&rend, sx, sy);
  }

  void setText (SDL_Renderer & rend, std::string str)
  {
    surface = TTF_RenderText_Solid(font, str.c_str(), SDL_Color {255, 255, 255, 255});
    texture = SDL_CreateTextureFromSurface(&rend, surface);

    // double w = surface->w;
    // double h = surface->h;
    // SDL_RenderCopy (&rend, texture, nullptr, V4 { x, y, w, h } . get());
    // SDL_FreeSurface(surface);
    // SDL_DestroyTexture(texture);
  }
  int delivered = 0;
  bool input () override {
    delivered++;

    if (hasText)
    {
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);
      hasText = false;
    }

    return true;
  }

private:
  static constexpr int fontSize = 36;
  TTF_Font    * font;
  SDL_Surface * surface = nullptr;
  SDL_Texture * texture = nullptr;
  bool hasText = false;

  void loadFont ()
  {
    font = TTF_OpenFont ("resources/DroidSans.ttf", fontSize);
  }
};

#endif // BASE_H
