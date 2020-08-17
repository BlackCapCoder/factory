#include <SDL2/SDL.h>
#include "World.h"
#include "Belt.h"
#include "Miner.h"
#include "Splitter.h"
#include "Trash.h"
#include "Undergroundee.h"
#include "Base.h"


void World::add (Entity        * e) { es.add(e); qt.insert (e); gr.updateAround (e); }
void World::add (Belt          * e) { bs.add(e); qt.insert (e); gr.updateAround (e); }
void World::add (Undergroundee * e) { us.add(e); qt.insert (e); gr.updateAround (e); }

void World::cleanAdd (Entity * e)
{
  std::vector<Entity*> es{};
  qt.getObjectsInBound
    ( V4 { (double) e->pos.x  + 0.1
         , (double) e->pos.y  + 0.1
         , (double) e->size.x - 0.2
         , (double) e->size.y - 0.2
         }
    , es
    );

  for (auto x : es) remove (x);

  add (e);
}

void World::remove (Belt * e) {
  // std::cout << "removing belt" << std::endl;
  qt.remove (e);
  gr.updateAround (e);
  bs.remove (e);
}
void World::remove (Entity * e) {
  if (auto * q = dynamic_cast<Belt*>(e); q != nullptr)
    remove (q);
  else
  if (auto * q = dynamic_cast<Undergroundee*>(e); q != nullptr)
    remove (q);
  else
  if (auto * q = dynamic_cast<Base*>(e); q != nullptr)
    return;
  else
  {
    // std::cout << "removing entity" << std::endl;
    qt.remove (e);
    gr.updateAround (e);
    es.remove (e);
  }
}
void World::remove (Undergroundee * e) {
  std::cout << "removing undergroundee" << std::endl;

  qt.remove (e);
  gr.updateAround (e);

  if (e->other != nullptr)
  {
    qt.remove (e->other);
    gr.updateAround (e->other);
    // us.remove (e->other);
  }
  // us.remove(e);
}

void World::tick (int dt)
{
  // gr.update();
  for (Belt     * e : bs) e->tick (*this, dt);
  for (Entity   * e : es) e->tick (*this, dt);
  for (auto     * e : us) e->tick (*this, dt);
  if (base != nullptr) base->tick (*this, dt);
}

void World::render (SDL_Renderer & rend, long time, float cx, float cy, int sw, int sh)
{
  V4 sr { cx-1, cy-1, (double) sw+2, (double) sh+2 };
  std::vector<Entity*> v{};

  qt.getObjectsInBound (sr, v);

  SDL_Rect r;


  for (Entity * x : v)
    if (auto e = dynamic_cast<Belt*>(x))
  {
    r.x = e->pos.x - cx;
    r.y = e->pos.y - cy;
    r.w = e->size.x;
    r.h = e->size.y;

    SDL_RenderSetViewport(&rend, &r);

    e->render (rend, time);
  }

  for (Entity * x : v)
    if (auto e = dynamic_cast<Belt*>(x))
  {
    r.x = e->pos.x - cx - 1;
    r.y = e->pos.y - cy - 1;
    r.w = e->size.x + 2;
    r.h = e->size.y + 2;

    SDL_RenderSetViewport(&rend, &r);

    e->renderTrack (rend, time);
  }

  for (Entity * x : v)
    if (auto e = dynamic_cast<Belt*>(x))
  {
    r.x = e->pos.x - cx - 1;
    r.y = e->pos.y - cy - 1;
    r.w = e->size.x + 2;
    r.h = e->size.y + 2;

    SDL_RenderSetViewport(&rend, &r);

    // e->renderTrack (rend, time);
    e->renderCargo (rend);
  }
  for (Entity * e : v)
    if (dynamic_cast<Belt*>(e) == nullptr)
  {
    r.x = e->pos.x - cx;
    r.y = e->pos.y - cy;
    r.w = e->size.x;
    r.h = e->size.y;

    SDL_RenderSetViewport(&rend, &r);

    e->render (rend, time);
  }

}

