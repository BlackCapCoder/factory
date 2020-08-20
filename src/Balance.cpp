#include "Balance.h"


std::string Balancers::serialize (World & world, int x, int y, int w, int h)
{
  std::vector<Entity*> es {};
  world.inside (x, y, w, h, es);

  const int s = w*h*2 + w;
  char str [s + 1];

  str[s] = 0;

  for (int i = 0; i < s; i++)
    str[i] = '.';

  for (int i = w*2; i < s; i += w*2 + 1)
    str[i] = '\n';

  for (auto q : es)
  {
    const int i = (q->pos.y - y) * (w * 2 + 1) + (q->pos.x - x) * 2;

    if (auto e = dynamic_cast<Belt*>(q))
    {
      str[i+0] = dir2chr (dswap(e->din));
      str[i+1] = dir2chr (e->dout);
    }
    else
    if (auto e = dynamic_cast<Splitter*>(q))
    {
      str[i+0] = 's';
      str[i+1] = dir2chr(e->dout);
    }
    else
    if (auto e = dynamic_cast<Undergroundee*>(q))
    {
      str[i+0] = e->entrance ? 'U' : 'u';
      str[i+1] = dir2chr(e->dir);
    }
  }

  return std::string (str);
}

