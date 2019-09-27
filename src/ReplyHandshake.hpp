#pragma once

#include "RequestHandshake.hpp"

namespace socks5
{
  class ReplyHandshake
  {
  public:
    ReplyHandshake(Method neededMethod);
    std::vector<uint8_t> GenBuff() const;
    std::string ToString() const;

  private:
    uint8_t _ver;
    uint8_t _method;
  };
}
