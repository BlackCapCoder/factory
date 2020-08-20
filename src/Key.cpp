#include "Key.h"


bool isKeyComposible (SDL_Keycode sym, SDL_Keymod mod)
{
  if (mod & KMOD_CTRL)
    return false;

  if (sym >= SDLK_a && sym <= SDLK_z)
    return true; // letter

  switch (sym)
  {
    case (SDLK_SPACE):        return true;
    case (SDLK_EXCLAIM):      return true;
    case (SDLK_QUOTEDBL):     return true;
    case (SDLK_HASH):         return true;
    case (SDLK_PERCENT):      return true;
    case (SDLK_DOLLAR):       return true;
    case (SDLK_AMPERSAND):    return true;
    case (SDLK_QUOTE):        return true;
    case (SDLK_LEFTPAREN):    return true;
    case (SDLK_RIGHTPAREN):   return true;
    case (SDLK_ASTERISK):     return true;
    case (SDLK_PLUS):         return true;
    case (SDLK_COMMA):        return true;
    case (SDLK_MINUS):        return true;
    case (SDLK_PERIOD):       return true;
    case (SDLK_SLASH):        return true;
    case (SDLK_0):            return true;
    case (SDLK_1):            return true;
    case (SDLK_2):            return true;
    case (SDLK_3):            return true;
    case (SDLK_4):            return true;
    case (SDLK_5):            return true;
    case (SDLK_6):            return true;
    case (SDLK_7):            return true;
    case (SDLK_8):            return true;
    case (SDLK_9):            return true;
    case (SDLK_COLON):        return true;
    case (SDLK_SEMICOLON):    return true;
    case (SDLK_LESS):         return true;
    case (SDLK_EQUALS):       return true;
    case (SDLK_GREATER):      return true;
    case (SDLK_QUESTION):     return true;
    case (SDLK_AT):           return true;
    case (SDLK_LEFTBRACKET):  return true;
    case (SDLK_BACKSLASH):    return true;
    case (SDLK_RIGHTBRACKET): return true;
    case (SDLK_CARET):        return true;
    case (SDLK_UNDERSCORE):   return true;
    case (SDLK_BACKQUOTE):    return true;
  }

  return false;
}

bool parseKeys (std::string str, std::vector<Key> & ks)
{
  for (int i = 0; i < str.size(); ++i)
  {
    // Trivial case
    if (str[i] != '<')
    {
      ks.push_back(Key (str[i]));
      continue;
    }

    int left = i; // opening <
    for (; i < str.size() && str[i] != '>'; i++);
    int right = i - 1; // closing >

    // unmatched < or <>, assume literal <
    if (str[i] != '>' || left == right)
    {
      ks.push_back (Key ('<'));
      i = left;
      continue;
    }

    left++;

    // j = final -
    int j = left + 1;
    for (; j < right && str[j] == '-'; j+=2);
    j -= 2;

    // letters between final - and closing >
    auto word = str.substr (j+1, right-j);

    if (word.size() == 1)
    {
      parseKeys (word, ks);
    }
    // else if (word.size() == 2 && j == left-1)
    // {
    //   ks.push_back (Key ('<'));
    //   i = j;
    //   continue;
    // }
    else
    {
      for (int k = 0; k < word.size(); ++k)
        word[k] &= 0xdf;

        static bool has_map = false;
        static std::map <std::string, SDL_Keycode> m {};

        if (!has_map)
        {
          has_map = true;

          m["RET"]         = SDLK_RETURN;
          m["RETURN"]      = SDLK_RETURN;
          m["ESC"]         = SDLK_ESCAPE;
          m["ESCAPE"]      = SDLK_ESCAPE;
          m["BACK"]        = SDLK_BACKSPACE;
          m["BACKSPACE"]   = SDLK_BACKSPACE;
          m["TAB"]         = SDLK_TAB;
          m["CAPS"]        = SDLK_CAPSLOCK;
          m["CAPSLOCK"]    = SDLK_CAPSLOCK;
          m["F1"]          = SDLK_F1;
          m["F2"]          = SDLK_F2;
          m["F3"]          = SDLK_F3;
          m["F4"]          = SDLK_F4;
          m["F5"]          = SDLK_F5;
          m["F6"]          = SDLK_F6;
          m["F7"]          = SDLK_F7;
          m["F8"]          = SDLK_F8;
          m["F9"]          = SDLK_F9;
          m["F10"]         = SDLK_F10;
          m["F11"]         = SDLK_F11;
          m["F12"]         = SDLK_F12;
          m["F13"]         = SDLK_F13;
          m["F14"]         = SDLK_F14;
          m["F15"]         = SDLK_F15;
          m["F16"]         = SDLK_F16;
          m["F17"]         = SDLK_F17;
          m["F18"]         = SDLK_F18;
          m["F19"]         = SDLK_F19;
          m["F20"]         = SDLK_F20;
          m["F21"]         = SDLK_F21;
          m["F22"]         = SDLK_F22;
          m["F23"]         = SDLK_F23;
          m["F24"]         = SDLK_F24;
          m["PRINTSCREEN"] = SDLK_PRINTSCREEN;
          m["SCROLLLOCK"]  = SDLK_SCROLLLOCK;
          m["PAUSE"]       = SDLK_PAUSE;
          m["INSERT"]      = SDLK_INSERT;
          m["HOME"]        = SDLK_HOME;
          m["PAGEUP"]      = SDLK_PAGEUP;
          m["PGUP"]        = SDLK_PAGEUP;
          m["PGDWN"]       = SDLK_PAGEDOWN;
          m["DEL"]         = SDLK_DELETE;
          m["DELETE"]      = SDLK_DELETE;
          m["END"]         = SDLK_END;
          m["RIGHT"]       = SDLK_RIGHT;
          m["LEFT"]        = SDLK_LEFT;
          m["UP"]          = SDLK_UP;
          m["DOWN"]        = SDLK_DOWN;
        }

        SDL_Keycode k = m[word];
        if (k == 0) return false; // unknown key

        ks.push_back(Key (k));
    }

    // Parse modifiers
    for (int u = left; u < j; u+=2)
    {
      Key & k = ks.back();

      switch (str[u] & 0xdf)
      {
        case 'C': k.mod = (SDL_Keymod) (k.mod | KMOD_CTRL); break;
        case 'S': k.mod = (SDL_Keymod) (k.mod | KMOD_SHIFT); break;
        case 'A': k.mod = (SDL_Keymod) (k.mod | KMOD_ALT); break;
        case 'M': k.mod = (SDL_Keymod) (k.mod | KMOD_GUI); break;
        case 'N': k.mod = (SDL_Keymod) (k.mod | KMOD_NUM); break;
        case 'K': k.mod = (SDL_Keymod) (k.mod | KMOD_CAPS); break;
        default:
          return false; // unknown modifier
      }
    }
  }

  return true;
}

