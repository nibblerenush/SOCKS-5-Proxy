#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class ReplyHmac
  {
  public:
    ReplyHmac(uint8_t status);
    std::vector<uint8_t> GenBuff() const;
    std::string ToString() const;
    
  private:
    uint8_t _ver;
    uint8_t _status;
  };
}
