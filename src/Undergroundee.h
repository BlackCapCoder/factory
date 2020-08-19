#ifndef UNDERGROUNDEE_H
#define UNDERGROUNDEE_H


#include <SDL2/SDL2_gfxPrimitives.h>
#include "Entity.h"
#include "World.h"
#include "Belt.h"


struct Undergroundee : Entity
{
  DIR  dir;
  bool entrance;
  bool cargo = false;
  Item item;
  Undergroundee * other = nullptr;

  // Maximum underground distance
  static constexpr int range = 4;


  Undergroundee (int x, int y, DIR dir, bool entrance)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dir   { dir }
    , entrance { entrance }
  {}

  void render (SDL_Renderer & rend, long time) override;

  void tick (World & w, int dt) override
  {
    if (entrance || !cargo)
      return;

    V2 p = pos + dir2V2 (dir);

    if (auto q = w.at(p.x, p.y); q != nullptr && q->input(item))
    {
      cargo = false;
      return;
    }
  }

  bool input (Item itm) override
  {
    if (entrance)
    {
      if (other == nullptr) return false;
      return other->input(itm);
    }
    else
    {
      if (cargo) return false;
      cargo = true;
      item  = itm;
      return true;
    }
  }

};

#endif // UNDERGROUNDEE_H
