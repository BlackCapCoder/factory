#ifndef KEYMAP_H
#define KEYMAP_H

#include "Key.h"
#include <map>
#include <vector>
#include <variant>
#include <functional>

typedef char Mode;
using SideEffect = std::function<void()>;

struct SubMap;
using KNode = std::variant<SubMap, SideEffect>;
struct SubMap : std::map <uint64_t, KNode> {};

struct KeyMap : std::map <Mode, SubMap>
{
  bool map (Mode mode, std::string keys, SideEffect f)
  {
    std::vector<Key> ks {};
    parseKeys (keys, ks);

    SubMap * m = &(*this)[mode];

    int i = 0;
    for (; i+1 < ks.size(); ++i)
    {
      if ((*m).find(ks[i].toUL()) == std::end(*m))
        (*m)[ks[i].toUL()] = { SubMap {} };
      m = &(std::get<SubMap>((*m)[ks[i].toUL()]));
    }

    (*m)[ks[i].toUL()] = { f };
    // std::cout << keys << ": " << m << std::endl;

    return true;
  }
};

struct KeySTM : KeyMap
{
  SubMap sm;
  SubMap fallback;

  void setMode (Mode m, Mode f)
  {
    sm       = (*this)[m];
    fallback = (*this)[f];
  }
  void setMode (Mode m)
  {
    setMode (m, m);
  }

  void put (Key k)
  {
    if (k.mod & KMOD_CTRL)
      k.mod = (SDL_Keymod) (k.mod|KMOD_CTRL);


    // std::cout << k.toString() << "\t" << k.toUL() << std::endl;

    auto ptr = sm.find(k.toUL());
    if (ptr == std::end(sm))
    {

      if ( k.sym == SDLK_LSHIFT || k.sym == SDLK_RSHIFT
        || k.sym == SDLK_LCTRL  || k.sym == SDLK_RCTRL
        || k.sym == SDLK_LALT   || k.sym == SDLK_RALT
        || k.sym == SDLK_LGUI   || k.sym == SDLK_RGUI
         )
        return;

      // std::cout << k.toString() << " not bound" << std::endl;

      sm = fallback;
      return;
    }

    KNode n = sm[k.toUL()];

    if (holds_alternative<SideEffect>(n))
    {
      sm = fallback;
      std::get<SideEffect>(n)();
    }
    else
    {
      sm = std::get<SubMap>(n);
      // std::cout << "sub: " << &sm << std::endl;
    }
  }

  void puts (std::string keys)
  {
    std::vector<Key> ks {};
    parseKeys(keys, ks);
    for (Key k : ks) put (k);
  }

  void exec (Mode m, std::string keys)
  {
    KeySTM st {};
    st.sm = (*this)[m];
    st.puts(keys);
  }

  void alias (Mode m, std::string a, std::string b)
  {
    map (m, a, [this, b](){ puts(b); });
  }

  void nmap (std::string a, std::string b)
  {
    alias ('n', a, b);
  }

  void debug ()
  {
    std::cout << "========== Keybinds ==========" << std::endl;
    for (auto [k, m] : sm)
    {
      unsigned long i = k;
      Key _k = *reinterpret_cast<Key*>(&i);
      std::cout << _k.toString() << "\t" << _k.toUL() << std::endl;
    }
    std::cout << "====================" << std::endl;
  }
};


#endif // KEYMAP_H
