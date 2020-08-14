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
};

#define dswap(d) ((DIR) ((d+2) & 3))

#define tileUnit 80


#endif // UTILS_H
