#ifndef _SOCKS_5_REPLY_USERNAME_PASSWORD_
#define _SOCKS_5_REPLY_USERNAME_PASSWORD_

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class Socks5ReplyUsernamePassword
  {
  public:
    Socks5ReplyUsernamePassword(uint8_t status);
    std::vector<uint8_t> GenerateReplyUsernamePasswordBuffer() const;
    std::string ToString() const;
  private:
    uint8_t _ver;
    uint8_t _status;
  };
}

#endif
