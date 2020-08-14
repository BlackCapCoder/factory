#ifndef UNDERGROUNDEE_H
#define UNDERGROUNDEE_H

#include "Entity.h"
#include "World.h"
#include "Belt.h"


struct Undergroundee : Entity
{
  DIR  dir;
  bool entrance;
  bool cargo = false;
  Undergroundee * other = nullptr;

  // Maximum underground distance
  static constexpr int range = 4;

  Undergroundee (int x, int y, DIR dir, bool entrance)
    : Entity (V2 {x, y}, V2 {1, 1})
    , dir   { dir }
    , entrance { entrance }
  {}

  void tick (World & w, int dt) override
  {
    if (entrance) return;
    if (!cargo) return;

    V2 p = pos;

    switch (dir)
    {
      case (DIR::N): p.y--; break;
      case (DIR::S): p.y++; break;
      case (DIR::W): p.x--; break;
      case (DIR::E): p.x++; break;
    }

    if (auto q = w.at(p.x, p.y); q != nullptr && q->input())
    {
      cargo = false;
      return;
    }
  }

  void render (SDL_Renderer & rend, long time) override
  {
    SDL_Rect r = { 0, 0, 1, 1 };

    SDL_SetRenderDrawColor (&rend, 64, 64, 16, 255);
    SDL_RenderFillRect (&rend, &r);
  }
  bool input () override {
    if (entrance)
    {
      if (other == nullptr) return false;
      return other->input();
    } else {
      if (cargo) return false;
      cargo = true;
      return true;
    }
  }
};

#endif // UNDERGROUNDEE_H
