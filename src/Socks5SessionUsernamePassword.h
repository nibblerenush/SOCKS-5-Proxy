#ifndef _SOCKS_5_SESSION_USERNAME_PASSWORD_H_
#define _SOCKS_5_SESSION_USERNAME_PASSWORD_H_

#include "Socks5SessionBase.h"

namespace socks5
{
  class Socks5SessionUsernamePassword : public Socks5SessionBase
  {
  public:
    Socks5SessionUsernamePassword(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);

  private:
    void Authenticate() override;
    virtual uint8_t GetAuthenticationMethod() const override;
    void ReadSocks5RequestUsernamePassword();
    void WriteSocks5RequestUsernamePassword();

  private:
    std::vector<uint8_t> _socks5RequestUsernamePasswordBuff;
    std::vector<uint8_t> _socks5ReplyUsernamePasswordBuff;
  };
}

#endif
