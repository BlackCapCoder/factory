#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <bitset>
#include <random>

struct WorldMap
{
  static constexpr int w = 1024;
  static constexpr int p = 13;
  std::bitset<w*w> buf;

  WorldMap ()
  {
    for (int i = 0; i < buf.size(); i++)
      buf[i] = std::rand() % 1000 <= p;
    for (int i = 0; i < 7; i++) rule45 ();
    invert();
    for (int i = 0; i < 7; i++) ruleFill ();
    for (int i = 0; i < 4; i++) ruleGrow();
  }

  bool tile (int x, int y)
  {
    static constexpr int q = w>>1;
    x = (x+q)%w; y = (y+q)%w;
    if (x < 0) x = w - x;
    if (y < 0) y = w - y;
    return bit (x, y);
  }

  bool bit (int x, int y)
  {
    if (x < 0 || y < 0 || x >= w || y >= w) return false;
    return buf[y*w+x];
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

  void rule45 ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);
        if (n == 0)
          tmp[y*w+x] = true;
        else
        if (n == 4)
          tmp[y*w+x] = buf[y*w+x];
        else
          tmp[y*w+x] = n >= 5;
      }
    buf = tmp;
  }

  void ruleGOL ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);
        if (n == 3)
          tmp[y*w+x] = true;
        else
        if (n == 2)
          tmp[y*w+x] = buf[y*w+x];
        else
          tmp[y*w+x] = false;
      }
    buf = tmp;
  }
  void ruleClean ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);
        if (n == 0 || n == 8)
          tmp[y*w+x] = false;
        else
          tmp[y*w+x] = buf[y*w+x];
      }
    buf = tmp;
  }
  void ruleFill ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);
        if (n >= 6)
          tmp[y*w+x] = true;
        else
          tmp[y*w+x] = buf[y*w+x];
      }
    buf = tmp;
  }
  void invert ()
  {
    for (int i = 0; i < w*w; i++)
      buf[i] = !buf[i];
  }
  void ruleGrow ()
  {
    decltype (buf) tmp;
    for (int x = 0; x < w; x++)
      for (int y = 0; y < w; y++)
      {
        int n = alives (x, y);
        tmp[y*w+x] = n >= 3;
      }
    buf = tmp;
  }

};

#endif // WORLDMAP_H
