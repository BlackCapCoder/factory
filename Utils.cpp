#include "Utils.h"

V2 dir2V2 (DIR d)
{
  switch (d)
  {
    case (DIR::N): return V2 {0, -1};
    case (DIR::S): return V2 {0, +1};
    case (DIR::W): return V2 {-1, 0};
    case (DIR::E): return V2 {+1, 0};
  }
}

int modulo (int x, int y)
{
  int z = x % y;
  if (z < 0) z = y - z;
  return z;
}

V2 V2::operator + (const V2 other)
{
  return V2 { x + other.x, y + other.y };
}
V2 V2::operator - (const V2 other)
{
  return V2 { x - other.x, y - other.y };
}

void V2::operator += (const V2 other)
{
  x += other.x;
  y += other.y;
}
void V2::operator -= (const V2 other)
{
  x -= other.x;
  y -= other.y;
}
