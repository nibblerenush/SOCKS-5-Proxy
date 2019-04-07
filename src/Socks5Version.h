#ifndef _SOCKS_5_VERSION_H_
#define _SOCKS_5_VERSION_H_

#include <cstdint>

namespace socks5
{
  class Socks5Version
  {
  public:
    static const uint8_t VER;
    static const uint8_t USERNAME_PASSWORD_VER;
  };
}

#endif


