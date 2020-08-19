#ifndef KEY_H
#define KEY_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>


bool isKeyComposible (SDL_Keycode sym, SDL_Keymod mod);


struct Key
{
  SDL_Keycode sym;
  SDL_Keymod  mod;

  Key (SDL_Keycode sym, SDL_Keymod mod) : sym {sym}, mod {mod}
  {}

  Key (char chr, SDL_Keymod mod) : sym {chr}, mod {mod}
  {}

  Key (SDL_Keycode sym) : sym {sym}, mod {KMOD_NONE}
  {}

  Key (char chr) : sym {chr}, mod {MOD_CHAR}
  {}


  std::string toString ()
  {
    std::string key = "";
    SDL_Keymod _mod = mod;
    bool special = false;

    if (isChar ())
    {
      const auto x = _mod & KMOD_SHIFT;
      key.push_back((char) sym);
      _mod = (SDL_Keymod) (_mod xor x);
      _mod = (SDL_Keymod) (_mod xor MOD_CHAR);
    } else
    if (isLetter())
    {
      const auto x = _mod & KMOD_SHIFT;
      key.push_back(x ? sym xor 0x20 : sym);
      _mod = (SDL_Keymod) (_mod xor x);
    } else
    {
      bool done = true;

      switch (sym)
      {
        case (SDLK_SPACE):      key.push_back (' ');  break;
        case (SDLK_EXCLAIM):    key.push_back ('!');  break;
        case (SDLK_QUOTEDBL):   key.push_back ('"');  break;
        case (SDLK_HASH):       key.push_back ('#');  break;
        case (SDLK_PERCENT):    key.push_back ('%');  break;
        case (SDLK_DOLLAR):     key.push_back ('$');  break;
        case (SDLK_AMPERSAND):  key.push_back ('&');  break;
        case (SDLK_QUOTE):      key.push_back ('\''); break;
        case (SDLK_LEFTPAREN):  key.push_back ('(');  break;
        case (SDLK_RIGHTPAREN): key.push_back (')');  break;
        case (SDLK_ASTERISK):   key.push_back ('*');  break;
        case (SDLK_PLUS):       key.push_back ('+');  break;
        case (SDLK_COMMA):      key.push_back (',');  break;
        case (SDLK_MINUS):      key.push_back ('-');  break;
        case (SDLK_PERIOD):     key.push_back ('.');  break;
        case (SDLK_SLASH):      key.push_back ('/');  break;
        case (SDLK_0):          key.push_back ('0');  break;
        case (SDLK_1):          key.push_back ('1');  break;
        case (SDLK_2):          key.push_back ('2');  break;
        case (SDLK_3):          key.push_back ('3');  break;
        case (SDLK_4):          key.push_back ('4');  break;
        case (SDLK_5):          key.push_back ('5');  break;
        case (SDLK_6):          key.push_back ('6');  break;
        case (SDLK_7):          key.push_back ('7');  break;
        case (SDLK_8):          key.push_back ('8');  break;
        case (SDLK_9):          key.push_back ('9');  break;
        case (SDLK_COLON):      key.push_back (':');  break;
        case (SDLK_SEMICOLON):  key.push_back (';');  break;
        case (SDLK_LESS):       key.push_back ('<');  break;
        case (SDLK_EQUALS):     key.push_back ('=');  break;
        case (SDLK_GREATER):    key.push_back ('>');  break;
        case (SDLK_QUESTION):   key.push_back ('?');  break;
        case (SDLK_AT):         key.push_back ('@');  break;

        case (SDLK_LEFTBRACKET):  key.push_back ('[');  break;
        case (SDLK_BACKSLASH):    key.push_back ('\\'); break;
        case (SDLK_RIGHTBRACKET): key.push_back (']');  break;
        case (SDLK_CARET):        key.push_back ('^');  break;
        case (SDLK_UNDERSCORE):   key.push_back ('_');  break;
        case (SDLK_BACKQUOTE):    key.push_back ('`');  break;

        default:
          done = false;
      }

      if (!done)
        special = true;

      if (!done) switch (sym)
      {
        case (SDLK_RETURN):             key.append("Ret");    break;
        case (SDLK_ESCAPE):             key.append("Esc");    break;
        case (SDLK_BACKSPACE):          key.append("Backspace"); break;
        case (SDLK_TAB):                key.append("Tab");       break;

        case (SDLK_CAPSLOCK):           key.append("Capslock"); _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_CAPS)); break;
        case (SDLK_F1):                 key.append("F1"); break;
        case (SDLK_F2):                 key.append("F2"); break;
        case (SDLK_F3):                 key.append("F3"); break;
        case (SDLK_F4):                 key.append("F4"); break;
        case (SDLK_F5):                 key.append("F5"); break;
        case (SDLK_F6):                 key.append("F6"); break;
        case (SDLK_F7):                 key.append("F7"); break;
        case (SDLK_F8):                 key.append("F8"); break;
        case (SDLK_F9):                 key.append("F9"); break;
        case (SDLK_F10):                key.append("F10"); break;
        case (SDLK_F11):                key.append("F11"); break;
        case (SDLK_F12):                key.append("F12"); break;
        case (SDLK_F13):                key.append("F13"); break;
        case (SDLK_F14):                key.append("F14"); break;
        case (SDLK_F15):                key.append("F15"); break;
        case (SDLK_F16):                key.append("F16"); break;
        case (SDLK_F17):                key.append("F17"); break;
        case (SDLK_F18):                key.append("F18"); break;
        case (SDLK_F19):                key.append("F19"); break;
        case (SDLK_F20):                key.append("F20"); break;
        case (SDLK_F21):                key.append("F21"); break;
        case (SDLK_F22):                key.append("F22"); break;
        case (SDLK_F23):                key.append("F23"); break;
        case (SDLK_F24):                key.append("F24"); break;
        case (SDLK_PRINTSCREEN):        key.append("Printscreen"); break;
        case (SDLK_SCROLLLOCK):         key.append("Scrolllock"); break;
        case (SDLK_PAUSE):              key.append("Pause"); break;
        case (SDLK_INSERT):             key.append("Insert"); break;
        case (SDLK_HOME):               key.append("Home"); break;
        case (SDLK_PAGEUP):             key.append("Pageup"); break;
        case (SDLK_DELETE):             key.append("Delete"); break;
        case (SDLK_END):                key.append("End"); break;
        case (SDLK_PAGEDOWN):           key.append("Pagedown"); break;
        case (SDLK_RIGHT):              key.append("Right"); break;
        case (SDLK_LEFT):               key.append("Left"); break;
        case (SDLK_DOWN):               key.append("Down"); break;
        case (SDLK_UP):                 key.append("Up"); break;
        case (SDLK_NUMLOCKCLEAR):       key.append("Numlockclear"); break;
        case (SDLK_KP_DIVIDE):          key.append("KP_DIVIDE"); break;
        case (SDLK_KP_MULTIPLY):        key.append("KP_MULTIPLY"); break;
        case (SDLK_KP_MINUS):           key.append("KP_MINUS"); break;
        case (SDLK_KP_PLUS):            key.append("KP_PLUS"); break;
        case (SDLK_KP_ENTER):           key.append("KP_ENTER"); break;
        case (SDLK_KP_1):               key.append("KP_1"); break;
        case (SDLK_KP_2):               key.append("KP_2"); break;
        case (SDLK_KP_3):               key.append("KP_3"); break;
        case (SDLK_KP_4):               key.append("KP_4"); break;
        case (SDLK_KP_5):               key.append("KP_5"); break;
        case (SDLK_KP_6):               key.append("KP_6"); break;
        case (SDLK_KP_7):               key.append("KP_7"); break;
        case (SDLK_KP_8):               key.append("KP_8"); break;
        case (SDLK_KP_9):               key.append("KP_9"); break;
        case (SDLK_KP_0):               key.append("KP_0"); break;
        case (SDLK_KP_PERIOD):          key.append("KP_PERIOD"); break;
        case (SDLK_APPLICATION):        key.append("Application"); break;
        case (SDLK_POWER):              key.append("Power"); break;
        case (SDLK_KP_EQUALS):          key.append("KP_EQUALS"); break;
        case (SDLK_EXECUTE):            key.append("Execute"); break;
        case (SDLK_HELP):               key.append("Help"); break;
        case (SDLK_MENU):               key.append("Menu"); break;
        case (SDLK_SELECT):             key.append("Select"); break;
        case (SDLK_STOP):               key.append("Stop"); break;
        case (SDLK_AGAIN):              key.append("Again"); break;
        case (SDLK_UNDO):               key.append("Undo"); break;
        case (SDLK_CUT):                key.append("Cut"); break;
        case (SDLK_COPY):               key.append("Copy"); break;
        case (SDLK_PASTE):              key.append("Paste"); break;
        case (SDLK_FIND):               key.append("Find"); break;
        case (SDLK_MUTE):               key.append("Mute"); break;
        case (SDLK_VOLUMEUP):           key.append("VOLUMEUP"); break;
        case (SDLK_VOLUMEDOWN):         key.append("VOLUMEDOWN"); break;
        case (SDLK_KP_COMMA):           key.append("KP_COMMA"); break;
        case (SDLK_KP_EQUALSAS400):     key.append("KP_EQUALSAS400"); break;
        case (SDLK_ALTERASE):           key.append("Alterase"); break;
        case (SDLK_SYSREQ):             key.append("Sysreq"); break;
        case (SDLK_CANCEL):             key.append("Cancel"); break;
        case (SDLK_CLEAR):              key.append("Clear"); break;
        case (SDLK_PRIOR):              key.append("Prior"); break;
        case (SDLK_RETURN2):            key.append("Return2"); break;
        case (SDLK_SEPARATOR):          key.append("Separator"); break;
        case (SDLK_OUT):                key.append("Out"); break;
        case (SDLK_OPER):               key.append("Oper"); break;
        case (SDLK_CLEARAGAIN):         key.append("Clearagain"); break;
        case (SDLK_CRSEL):              key.append("CRSEL"); break;
        case (SDLK_EXSEL):              key.append("EXSEL"); break;
        case (SDLK_KP_00):              key.append("KP_00"); break;
        case (SDLK_KP_000):             key.append("KP_000"); break;
        case (SDLK_THOUSANDSSEPARATOR): key.append("THOUSANDSSEPARATOR"); break;
        case (SDLK_DECIMALSEPARATOR):   key.append("DECIMALSEPARATOR"); break;
        case (SDLK_CURRENCYUNIT):       key.append("CURRENCYUNIT"); break;
        case (SDLK_CURRENCYSUBUNIT):    key.append("CURRENCYSUBUNIT"); break;
        case (SDLK_KP_LEFTPAREN):       key.append("KP_LEFTPAREN"); break;
        case (SDLK_KP_RIGHTPAREN):      key.append("KP_RIGHTPAREN"); break;
        case (SDLK_KP_LEFTBRACE):       key.append("KP_LEFTBRACE"); break;
        case (SDLK_KP_RIGHTBRACE):      key.append("KP_RIGHTBRACE"); break;
        case (SDLK_KP_TAB):             key.append("KP_TAB"); break;
        case (SDLK_KP_BACKSPACE):       key.append("KP_BACKSPACE"); break;
        case (SDLK_KP_A):               key.append("KP_A"); break;
        case (SDLK_KP_B):               key.append("KP_B"); break;
        case (SDLK_KP_C):               key.append("KP_C"); break;
        case (SDLK_KP_D):               key.append("KP_D"); break;
        case (SDLK_KP_E):               key.append("KP_E"); break;
        case (SDLK_KP_F):               key.append("KP_F"); break;
        case (SDLK_KP_XOR):             key.append("KP_XOR"); break;
        case (SDLK_KP_POWER):           key.append("KP_POWER"); break;
        case (SDLK_KP_PERCENT):         key.append("KP_PERCENT"); break;
        case (SDLK_KP_LESS):            key.append("KP_LESS"); break;
        case (SDLK_KP_GREATER):         key.append("KP_GREATER"); break;
        case (SDLK_KP_AMPERSAND):       key.append("KP_AMPERSAND"); break;
        case (SDLK_KP_DBLAMPERSAND):    key.append("KP_DBLAMPERSAND"); break;
        case (SDLK_KP_VERTICALBAR):     key.append("KP_VERTICALBAR"); break;
        case (SDLK_KP_DBLVERTICALBAR):  key.append("KP_DBLVERTICALBAR"); break;
        case (SDLK_KP_COLON):           key.append("KP_COLON"); break;
        case (SDLK_KP_HASH):            key.append("KP_HASH"); break;
        case (SDLK_KP_SPACE):           key.append("KP_SPACE"); break;
        case (SDLK_KP_AT):              key.append("KP_AT"); break;
        case (SDLK_KP_EXCLAM):          key.append("KP_EXCLAM"); break;
        case (SDLK_KP_MEMSTORE):        key.append("KP_MEMSTORE"); break;
        case (SDLK_KP_MEMRECALL):       key.append("KP_MEMRECALL"); break;
        case (SDLK_KP_MEMCLEAR):        key.append("KP_MEMCLEAR"); break;
        case (SDLK_KP_MEMADD):          key.append("KP_MEMADD"); break;
        case (SDLK_KP_MEMSUBTRACT):     key.append("KP_MEMSUBTRACT"); break;
        case (SDLK_KP_MEMMULTIPLY):     key.append("KP_MEMMULTIPLY"); break;
        case (SDLK_KP_MEMDIVIDE):       key.append("KP_MEMDIVIDE"); break;
        case (SDLK_KP_PLUSMINUS):       key.append("KP_PLUSMINUS"); break;
        case (SDLK_KP_CLEAR):           key.append("KP_CLEAR"); break;
        case (SDLK_KP_CLEARENTRY):      key.append("KP_CLEARENTRY"); break;
        case (SDLK_KP_BINARY):          key.append("KP_BINARY"); break;
        case (SDLK_KP_OCTAL):           key.append("KP_OCTAL"); break;
        case (SDLK_KP_DECIMAL):         key.append("KP_DECIMAL"); break;
        case (SDLK_KP_HEXADECIMAL):     key.append("KP_HEXADECIMAL"); break;
        case (SDLK_LCTRL):              key.append("LCTRL");  _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_LCTRL)); break;
        case (SDLK_LSHIFT):             key.append("LSHIFT"); _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_LSHIFT)); break;
        case (SDLK_LALT):               key.append("LALT");   _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_LALT)); break;
        case (SDLK_LGUI):               key.append("LGUI");   _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_LGUI)); break;
        case (SDLK_RCTRL):              key.append("RCTRL");  _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_RCTRL)); break;
        case (SDLK_RSHIFT):             key.append("RSHIFT"); _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_RSHIFT)); break;
        case (SDLK_RALT):               key.append("RALT");   _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_RALT)); break;
        case (SDLK_RGUI):               key.append("RGUI");   _mod = (SDL_Keymod) (_mod xor (_mod & KMOD_RGUI)); break;
        case (SDLK_MODE):               key.append("MODE"); break;
        case (SDLK_AUDIONEXT):          key.append("AUDIONEXT"); break;
        case (SDLK_AUDIOPREV):          key.append("AUDIOPREV"); break;
        case (SDLK_AUDIOSTOP):          key.append("AUDIOSTOP"); break;
        case (SDLK_AUDIOPLAY):          key.append("AUDIOPLAY"); break;
        case (SDLK_AUDIOMUTE):          key.append("AUDIOMUTE"); break;
        case (SDLK_MEDIASELECT):        key.append("MEDIASELECT"); break;
        case (SDLK_WWW):                key.append("WWW"); break;
        case (SDLK_MAIL):               key.append("MAIL"); break;
        case (SDLK_CALCULATOR):         key.append("CALCULATOR"); break;
        case (SDLK_COMPUTER):           key.append("COMPUTER"); break;
        case (SDLK_AC_SEARCH):          key.append("AC_SEARCH"); break;
        case (SDLK_AC_HOME):            key.append("AC_HOME"); break;
        case (SDLK_AC_BACK):            key.append("AC_BACK"); break;
        case (SDLK_AC_FORWARD):         key.append("AC_FORWARD"); break;
        case (SDLK_AC_STOP):            key.append("AC_STOP"); break;
        case (SDLK_AC_REFRESH):         key.append("AC_REFRESH"); break;
        case (SDLK_AC_BOOKMARKS):       key.append("AC_BOOKMARKS"); break;
        case (SDLK_BRIGHTNESSDOWN):     key.append("BRIGHTNESSDOWN"); break;
        case (SDLK_BRIGHTNESSUP):       key.append("BRIGHTNESSUP"); break;
        case (SDLK_DISPLAYSWITCH):      key.append("DISPLAYSWITCH"); break;
        case (SDLK_KBDILLUMTOGGLE):     key.append("KBDILLUMTOGGLE"); break;
        case (SDLK_KBDILLUMDOWN):       key.append("KBDILLUMDOWN"); break;
        case (SDLK_KBDILLUMUP):         key.append("KBDILLUMUP"); break;
        case (SDLK_EJECT):              key.append("EJECT"); break;
        case (SDLK_SLEEP):              key.append("SLEEP"); break;
        case (SDLK_APP1):               key.append("APP1"); break;
        case (SDLK_APP2):               key.append("APP2"); break;
        case (SDLK_AUDIOREWIND):        key.append("AUDIOREWIND"); break;
        case (SDLK_AUDIOFASTFORWARD):   key.append("AUDIOFASTFORWARD"); break;

        default:                        key.append("UNKNOWN"); break;
      }
    }

    if (_mod != 0)
    {
      std::string mods = "";
      special = true;

      if (_mod & KMOD_SHIFT) mods.append ("S-");
      if (_mod & KMOD_CTRL)  mods.append ("C-");
      if (_mod & KMOD_ALT)   mods.append ("A-");
      if (_mod & KMOD_GUI)   mods.append ("M-");
      if (_mod & KMOD_NUM)   mods.append ("N-");
      if (_mod & KMOD_CAPS)  mods.append ("K-");
      // if (_mod & KMOD_MODE)  mods.append ("M-");

      mods.append (key);
      key = mods;
    }

    if (special)
      key = "<" + key + ">";

    return key;
  }

  inline bool operator == (Key other)
  {
    return toUL() == other.toUL();
  }

  bool operator < (Key other)
  {
    return toUL() < other.toUL();
  }

  bool isChar ()
  {
    return mod & MOD_CHAR;
  }

  static constexpr SDL_Keymod MOD_CHAR
    = KMOD_RESERVED;

  inline uint64_t toUL ()
  {
    return *reinterpret_cast<uint64_t*>(this);
  }

private:

  constexpr bool isLetter ()
  {
    return sym >= SDLK_a && sym <= SDLK_z;
  }
};


bool parseKeys (std::string str, std::vector<Key> & ks);


#endif // KEY_H
