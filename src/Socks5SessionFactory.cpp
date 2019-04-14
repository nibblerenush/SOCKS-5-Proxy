#include "Socks5SessionFactory.h"
#include "Socks5Exception.h"
#include "Socks5SessionNoAuthentication.h"
#include "Socks5SessionUsernamePassword.h"

socks5::Socks5RequestHandshake::Method socks5::Socks5SessionFactory::_Type = socks5::Socks5RequestHandshake::NO_AUTHENTICATION_REQUIRED;
const uint16_t socks5::Socks5SessionFactory::DEFAULT_BUFFER_SIZE = 1024;

void socks5::Socks5SessionFactory::SetType(Socks5RequestHandshake::Method Type)
{
  _Type = Type;
}

std::shared_ptr<socks5::Socks5SessionBase> socks5::Socks5SessionFactory::CreateSession(
  ba::ip::tcp::socket && socket,
  const bpt::ptree & configurationFile,
  int sessionId)
{
  switch (_Type)
  {
  case Socks5RequestHandshake::NO_AUTHENTICATION_REQUIRED:
    return std::make_shared<Socks5SessionNoAuthentication>(
      std::move(socket),
      configurationFile.get<uint16_t>("buffer.size", DEFAULT_BUFFER_SIZE),
      sessionId);
  case Socks5RequestHandshake::USERNAME_PASSWORD:
    return std::make_shared<Socks5SessionUsernamePassword>(
      std::move(socket),
      configurationFile.get<std::string>("username_password.username"),
      configurationFile.get<std::string>("username_password.password"),
      configurationFile.get<uint16_t>("buffer.size", DEFAULT_BUFFER_SIZE),
      sessionId);
  default:
    throw Socks5Exception("Unknown method!");
  }
}
