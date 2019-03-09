#ifndef _SOCKS_5_REPLY_HANDSHAKE_H_
#define _SOCKS_5_REPLY_HANDSHAKE_H_

#include "Socks5RequestHandshake.h"

namespace socks5
{
  class Socks5ReplyHandshake
  {
  public:
    Socks5ReplyHandshake(const Socks5RequestHandshake & socks5RequestHandshake);
    std::vector<uint8_t> GenerateReplyHandshakeBuffer() const;
    std::string ToString() const;
  private:
    uint8_t _ver;
    uint8_t _method;
  };
}

#endif
