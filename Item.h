#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>

enum Color
{
  Gray  = 0,
  Red   = 1,
  Green = 2,
  Blue  = 4,
};

struct Item
{
  Color color = Gray;

  void render (SDL_Renderer & rend, SDL_FRect & r)
  {
    char R = 96
       , G = 96
       , B = 96;

    if (color & Red)   R = 255;
    if (color & Green) G = 255;
    if (color & Blue)  B = 255;

    SDL_SetRenderDrawColor (&rend, R, G, B, 255);
    SDL_RenderFillRectF (&rend, &r);
  }

  bool operator == (Item other)
  {
    return color == other.color;
  }
};


#endif // ITEM_H
