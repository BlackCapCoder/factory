#ifndef TRASH_H
#define TRASH_H

#include "Entity.h"
#include "World.h"
#include "Belt.h"

struct Trash : public Entity
{
  Trash (int x, int y)
    : Entity (V2 {x, y}, V2 {1, 1})
  {}

  void tick (World & w, int dt) override
  {
  }

  void render (SDL_Renderer & rend, long time) override;

  bool input (Item) override { return true; }
};

#endif // TRASH_H
