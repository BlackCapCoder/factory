#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

enum Color
{
  Gray  = 0,
  Red   = 1,
  Green = 2,
  Blue  = 4,
};
enum Shape
{
  Void   = 0,
  Square = 1,
  Circle = 2,
  Star   = 3,
};

struct Quad
{
  Color color { Gray   };
  Shape shape { Square };

  bool operator == (Quad other)
  {
    return color == other.color
        && shape == other.shape
         ;
  }
};

struct Item
{
  Quad quads[4];

public:

  constexpr Item ()             = default;
  constexpr Item (const Item &) = default;

  constexpr Item (const Quad tl, const Quad tr, const Quad br, const Quad bl)
    : quads { tl, tr, br, bl }
  {}

  constexpr Item (const Quad qs[4])
    : Item ( qs[0], qs[1], qs[2], qs[3] )
  {}

  constexpr Item (const Quad q)
    : Item ( q, q, q, q )
  {}

  constexpr Item (const Color c, const Shape s)
    : Item ( Quad { c, s } )
  {}


public:

  void renderQuad (SDL_Renderer & rend, Quad q, const int ix, const float cx, const float cy, const SDL_FRect & r)
  {
    if (q.shape == Shape::Void)
      return;

    char R = 96
       , G = 96
       , B = 96;

    if (q.color & Red)   R = 255;
    if (q.color & Green) G = 255;
    if (q.color & Blue)  B = 255;

    SDL_SetRenderDrawColor (&rend, R, G, B, 255);

    switch (q.shape)
    {
      case (Shape::Square):
      {
        SDL_RenderFillRectF (&rend, &r);
        break;
      }

      case (Shape::Circle):
      {
        float sx, sy;
        SDL_RenderGetScale (&rend, &sx, &sy);
        SDL_RenderSetScale (&rend, 1.0, 1.0);
        fillCircle (&rend, cx*sx, cy*sy, r.w*sx, 3 << (ix << 1));
        SDL_RenderSetScale (&rend, sx, sy);
        break;
      }

      case (Shape::Star):
      {
        float sx, sy;
        SDL_RenderGetScale (&rend, &sx, &sy);
        SDL_RenderSetScale (&rend, 1.0, 1.0);
        switch (ix)
        {
          case 0: // BR
            filledTrigonRGBA (&rend, (r.x+r.w)*sx, (r.y+r.h)*sy, (r.x + r.w/2)*sx, r.y*sy, r.x*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            filledTrigonRGBA (&rend, cx*sx, cy*sy, (r.x + r.w/2)*sx, r.y*sy, r.x*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            break;
          case 1: // BL
            filledTrigonRGBA (&rend, r.x*sx, (r.y+r.h)*sy, (r.x + r.w/2)*sx, r.y*sy, (r.x+r.w)*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            filledTrigonRGBA (&rend, cx*sx, cy*sy, (r.x + r.w/2)*sx, r.y*sy, (r.x+r.w)*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            break;
          case 2: // TL
            filledTrigonRGBA (&rend, r.x*sx, r.y*sy, (r.x + r.w/2)*sx, (r.y+r.h)*sy, (r.x + r.w)*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            filledTrigonRGBA (&rend, cx*sx, cy*sy, (r.x + r.w/2)*sx, (r.y+r.h)*sy, (r.x + r.w)*sx, (r.y + r.h/2)*sy, R, G, B, 255);
            break;
          case 3: // TR
            filledTrigonRGBA (&rend, (r.x+r.w)*sx, r.y*sy, r.x*sx, (r.y+r.h/2)*sy, (r.x+r.w/2)*sx, (r.y+r.h)*sy, R, G, B, 255);
            filledTrigonRGBA (&rend, cx*sx, cy*sy, r.x*sx, (r.y+r.h/2)*sy, (r.x+r.w/2)*sx, (r.y+r.h)*sy, R, G, B, 255);
            break;
        }
        SDL_RenderSetScale (&rend, sx, sy);
        break;
      }
    }
  }

  void render (SDL_Renderer & rend, SDL_FRect & r)
  {
    const float w = r.w/2;
    const float h = r.h/2;

    const float cx = r.x + w;
    const float cy = r.y + h;

    renderQuad (rend, quads[0], 0, cx, cy, SDL_FRect {r.x+w, r.y+h, w, h}); // BR
    renderQuad (rend, quads[1], 1, cx, cy, SDL_FRect {r.x,   r.y+h, w, h}); // BL
    renderQuad (rend, quads[2], 2, cx, cy, SDL_FRect {r.x,   r.y,   w, h}); // TL
    renderQuad (rend, quads[3], 3, cx, cy, SDL_FRect {r.x+w, r.y,   w, h}); // TR
  }

  bool operator == (Item other)
  {
    return quads[0] == other.quads[0]
        && quads[1] == other.quads[1]
        && quads[2] == other.quads[2]
        && quads[3] == other.quads[3]
         ;
  }


private:

  void fillCircle (SDL_Renderer * rend, int32_t const cx, int32_t const cy, int32_t const radius, unsigned char const quads)
  {
    const int32_t diameter = radius * 2;

    int32_t x     = radius - 1;
    int32_t y     = 0;
    int32_t tx    = 1;
    int32_t ty    = 1;
    int32_t error = tx - diameter;

    do {
      for (; error <= 0; ++y, error += ty, ty += 2)
        switch (quads & 0b1100110)
        {
          case (1 << 1):
            SDL_RenderDrawLine (rend, cx + y, cy, cx + y, cy + x); break;
          case (1 << 2):
            SDL_RenderDrawLine (rend, cx - y, cy, cx - y, cy + x); break;
          case (1 << 5):
            SDL_RenderDrawLine (rend, cx - y, cy, cx - y, cy - x); break;
          case (1 << 6):
            SDL_RenderDrawLine (rend, cx + y, cy, cx + y, cy - x); break;
        };

      for (; error > 0; --x, tx += 2, error += tx - diameter)
        switch (quads & 0b10011001)
        {
          case (1 << 0):
            SDL_RenderDrawLine (rend, cx + x, cy, cx + x, cy + y); break;
          case (1 << 3):
            SDL_RenderDrawLine (rend, cx - x, cy, cx - x, cy + y); break;
          case (1 << 4):
            SDL_RenderDrawLine (rend, cx - x, cy, cx - x, cy - y); break;
          case (1 << 7):
            SDL_RenderDrawLine (rend, cx + x, cy, cx + x, cy - y); break;
        };

    } while (x >= y);
  }

};


#endif // ITEM_H
