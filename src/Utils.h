#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>


enum DIR { N, E, S, W };

struct Camera
{
  float x, y, z;
};

struct V2
{
  int x, y;

  V2 operator + (const V2) const;
  V2 operator - (const V2);

  void operator += (const V2 other);
  void operator -= (const V2 other);
};

#define dswap(d) ((DIR) ((d+2) & 3))

#define tileUnit 80


V2  dir2V2 (DIR);
int modulo (int x, int y);


#endif // UTILS_H
