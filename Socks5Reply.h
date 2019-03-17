#ifndef _SOCKS_5_REPLY_H_
#define _SOCKS_5_REPLY_H_

#include "Socks5Request.h"

namespace socks5
{
  class Socks5Reply
  {
  public:
    enum Rep
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
  
  public:
    Socks5Reply(uint32_t realRemoteIp, uint16_t realRemotePort);
    std::vector<uint8_t> GenerateReplyBuffer() const;
    std::string ToString() const;

  private:
    uint8_t _ver;
    uint8_t _rep;
    uint8_t _rsv;
    uint8_t _atyp;
    
    uint32_t _realRemoteIp;
    uint16_t _realRemotePort;

  private:
    static const uint8_t RESERVED_VALUE;
    static const uint8_t REPLY_BUFFER_SIZE;
  };
}

#endif
