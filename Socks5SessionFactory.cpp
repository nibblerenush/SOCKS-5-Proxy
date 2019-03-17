#include "Socks5SessionFactory.h"
#include "Socks5Exception.h"
#include "Socks5SessionNoAuthentication.h"

socks5::Socks5RequestHandshake::Method socks5::Socks5SessionFactory::_Type = socks5::Socks5RequestHandshake::NO_AUTHENTICATION_REQUIRED;

void socks5::Socks5SessionFactory::SetType(Socks5RequestHandshake::Method Type)
{
  _Type = Type;
}

std::shared_ptr<socks5::Socks5SessionBase> socks5::Socks5SessionFactory::CreateSession(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId)
{
  switch (_Type)
  {
  case Socks5RequestHandshake::NO_AUTHENTICATION_REQUIRED:
    return std::make_shared<Socks5SessionNoAuthentication>(std::move(socket), bufferSize, sessionId);
  default:
    throw Socks5Exception("Unknown method!");
  }
}
