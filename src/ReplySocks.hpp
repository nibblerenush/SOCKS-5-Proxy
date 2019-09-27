#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  enum Rep : uint8_t
  {
    SUCCEEDED = 0x00,
    GENERAL_SOCKS_SERVER_FAILURE = 0x01,
    CONNECTION_NOT_ALLOWED_BY_RULESET = 0x02,
    NETWORK_UNREACHABLE = 0x03,
    HOST_UNREACHABLE = 0x04,
    CONNECTION_REFUSED = 0x05,
    TTL_EXPIRED = 0x06,
    COMMAND_NOT_SUPPORTED = 0x07,
    ADDRESS_TYPE_NOT_SUPPORTED = 0x08
    // From 0x09 to 0xFF unassigned
  };
  
  class ReplySocks
  {
  public:
    ReplySocks(uint32_t bndAddr, uint16_t bndPort);
    std::vector<uint8_t> GenBuff() const;
    std::string ToString() const;
    
  private:
    uint8_t _ver;
    uint8_t _rep;
    uint8_t _rsv;
    uint8_t _atyp;
    
    uint32_t _bndAddr;
    uint16_t _bndPort;
  };
}
