#ifndef _SOCKS_5_NO_AUTHENTICATION_H_
#define _SOCKS_5_NO_AUTHENTICATION_H_

#include "Socks5SessionBase.h"

namespace socks5
{
  class Socks5SessionNoAuthentication : public Socks5SessionBase
  {
  public:
    Socks5SessionNoAuthentication(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);
  private:
    void Authenticate() override;
    virtual uint8_t GetAuthenticationMethod() const override;
  };
}

#endif
