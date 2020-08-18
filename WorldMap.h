#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <bitset>
#include <random>
#include "Item.h"
#include "Utils.h"

struct WorldMap
{
  static constexpr int w    = 1024;
  static constexpr int size = 1 << 4;

  std::bitset<w*w> buf;

  WorldMap ()
  {
    for (int i = 0; i < buf.size(); i++)
      buf[i] = std::rand() % 100 <= 45;

    bigRule45<8> (28, 4);
    bigRule45<4> (8,  2);

    for (int i = 0; i < 4; i++)
      rule45 ();

    bigRule45<4> (7, 2);

    for (int i = 0; i < 8; i++)
    {
      bigSieve (8, 32);
      rule45 ();
    }

    for (int i = 0; i < 4; i++)
      rule45 ();

    paint ();
  }


  static constexpr int rsize = size;
  unsigned short pbuf [(w*w)/(rsize*rsize)] {};

  void paint ()
  {
    static constexpr int u = w/rsize;

    std::bitset<u*u> bs {};

    for (int y = 0; y < u; ++y)
    for (int x = 0; x < u; ++x)
    {
      for (int oy = 0; oy < rsize; ++oy)
      for (int ox = 0; ox < rsize; ++ox)
      if (buf[oy*w + ox + x*rsize + y*rsize*w])
      {
        bs[y*u + x] = true;
        goto DONE;
      }
DONE:
      {}
    }

    for (int y = 0; y < u; ++y)
    for (int x = 0; x < u; ++x)
    {
      const int i = y*u + x;

      if (!bs [i]) continue;

      unsigned short val;

      if (pbuf [i])
        val = pbuf [i];
      else
        val = pbuf [i] = std::rand ();

      if (x > 0) pbuf [i - 1] = val;
      if (x < u) pbuf [i + 1] = val;
      if (y > 0) pbuf [i - u] = val;
      if (y < u) pbuf [i + u] = val;
    }
  }

  bool tile (int x, int y)
  {
    static constexpr int q = w>>1;
    x = (x+q)%w; y = (y+q)%w;
    if (x < 0) x = w - x;
    if (y < 0) y = w - y;
    if (x < 0 || y < 0 || x >= w || y >= w) return false;
    return bit (x, y);
  }
  Item resource (int x, int y)
  {
    static constexpr int q = w>>1;
    static constexpr int u = w/rsize;

    x = (x+q)%w; y = (y+q)%w;
    if (x < 0) x = w - x;
    if (y < 0) y = w - y;

    const int k = pbuf[(y/rsize)*u + x/rsize];

    int  s1      =  (k >> 2*0) & 3;
    bool isPaint = ((k >> 2*1) & 3) == 3;
    int  s2      = s1;
    int  s3      = s1;
    int  s4      = s1;
    int  color   = Color::Gray;

    if (!isPaint)
    {
      if (((k >> 2*2) & 3) == 3) s2 = (k >> 2*3) & 3;
      if (((k >> 2*4) & 3) == 3) s2 = (k >> 2*5) & 3;
      if (((k >> 2*6) & 3) == 3) s3 = (k >> 2*7) & 3;
    } else
    switch ((k >> 2*2) & 3)
    {
      // case 0: color = Color::Gray;  break;
      case 0: color = Color::Red;   break;
      case 1: color = Color::Red;   break;
      case 2: color = Color::Green; break;
      case 3: color = Color::Blue;  break;
    }

    if (s1 == Shape::Void) s1 = Shape::Square;
    if (s2 == Shape::Void) s2 = Shape::Square;
    if (s3 == Shape::Void) s3 = Shape::Square;
    if (s4 == Shape::Void) s4 = Shape::Square;

    if (!isPaint)
      return Item { Quad { (Color) color, (Shape) s1 }
                  , Quad { (Color) color, (Shape) s2 }
                  , Quad { (Color) color, (Shape) s3 }
                  , Quad { (Color) color, (Shape) s4 } };

    return Item
      { (Color) color };
  }

  bool bit (int x, int y)
  {
    if (x < 0 || y < 0 || x >= w || y >= w) return false;
    return buf[y*w+x];
  }

  template <int w>
  bool bitG (std::bitset<w*w> & buf, int x, int y)
  {
    if (x < 0 || y < 0 || x >= w || y >= w) return false;
    return buf[y*w+x];
  }

  int bigPopcnt (int size, int x, int y)
  {
    const int u = w/size;

    if (x < 0 || y < 0 || x >= u || y >= u)
      return 0;

    int cnt {};

    for (int _y = y*size; _y < (y+1)*size; _y++)
    for (int _x = x*size; _x < (x+1)*size; _x++)
    if  (bit (_x, _y))
      cnt++;

    return cnt;
  }
  bool bigBit (int size, int lim, int x, int y)
  {
    return bigPopcnt (size, x, y) >= lim;
  }

  void setBigTile (int size, int x, int y, bool val)
  {
    for (int _y = y*size; _y < (y+1)*size; _y++)
      for (int _x = x*size; _x < (x+1)*size; _x++)
        buf[_y*w + _x] = val;
  }

  void bigSieve (int size, int lim)
  {
    const int u = w/size;

    for (int y = 0; y < u; ++y)
    for (int x = 0; x < u; ++x)
    if  (bigPopcnt (size, x, y) >= lim)
      setBigTile (size, x, y, false);
  }


  int alives (int x, int y)
  {
    int cnt = 0;
    if (bit (x-1, y-1)) cnt++;
    if (bit (x  , y-1)) cnt++;
    if (bit (x+1, y-1)) cnt++;
    if (bit (x-1, y  )) cnt++;
    if (bit (x+1, y  )) cnt++;
    if (bit (x-1, y+1)) cnt++;
    if (bit (x  , y+1)) cnt++;
    if (bit (x+1, y+1)) cnt++;
    return cnt;
  }

  template <int w>
  int alivesG (std::bitset<w*w> & buf, int x, int y)
  {
    int cnt = 0;
    if (bitG <w> (buf, x-1, y-1)) cnt++;
    if (bitG <w> (buf, x  , y-1)) cnt++;
    if (bitG <w> (buf, x+1, y-1)) cnt++;
    if (bitG <w> (buf, x-1, y  )) cnt++;
    if (bitG <w> (buf, x+1, y  )) cnt++;
    if (bitG <w> (buf, x-1, y+1)) cnt++;
    if (bitG <w> (buf, x  , y+1)) cnt++;
    if (bitG <w> (buf, x+1, y+1)) cnt++;
    return cnt;
  }

  void rule45 ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);

        if (buf[y*w+x])
          tmp[y*w+x] = n >= 4;
        else
          tmp[y*w+x] = n >= 5;
      }
    buf = tmp;
  }

  template <int size>
  void bigRule45 (int lim, int ittr)
  {
    constexpr int u = w/size;

    std::bitset<(w*w)/(size*size)> bs;

    for (int y = 0; y < u; y++)
    for (int x = 0; x < u; x++)
      bs[y*u+x] = bigBit (size, lim, x, y);

    for (int i = 0; i < ittr; i++)
    {
      std::bitset<(w*w)/(size*size)> tmp;

      for (int y = 0; y < u; y++)
      for (int x = 0; x < u; x++)
      {
        int n = alivesG<u> (bs, x, y);

        if (bs[y*u+x])
          tmp[y*u+x] = n >= 4;
        else
          tmp[y*u+x] = n >= 5;
      }

      bs = tmp;
    }

    for (int y = 0; y < u; y++)
    for (int x = 0; x < u; x++)
    {
      if  (!bs[y*u+x])
        setBigTile (size, x, y, false);
    }
  }

};

#endif // WORLDMAP_H
