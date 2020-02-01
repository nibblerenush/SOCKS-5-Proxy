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
    
  public:
    static const uint8_t SUCCESS;
    static const uint8_t FAILURE;
    
  private:
    uint8_t _ver;
    uint8_t _status;
  };
}
