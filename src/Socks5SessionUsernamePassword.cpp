#include "Socks5SessionUsernamePassword.h"
#include "ErrorPrinter.h"
#include "Socks5Exception.h"
#include "Socks5RequestHandshake.h"

#include <iostream>

socks5::Socks5SessionUsernamePassword::Socks5SessionUsernamePassword(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId) :
  Socks5SessionBase(std::move(socket), bufferSize, sessionId),
  _socks5RequestUsernamePasswordBuff(bufferSize)
{}

void socks5::Socks5SessionUsernamePassword::Authenticate()
{
  ReadSocks5RequestUsernamePassword();
}

void socks5::Socks5SessionUsernamePassword::ReadSocks5RequestUsernamePassword()
{
  auto self(shared_from_this());
  _inSocket.async_receive
  (
    ba::buffer(_socks5RequestUsernamePasswordBuff, _socks5RequestUsernamePasswordBuff.size()),
    [this, self](const boost::system::error_code & errorCode, std::size_t readedLength)
    {
      if (!errorCode)
      {
        try
        {

        }
        catch (socks5::Socks5Exception socks5Exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5RequestUsernamePassword)", socks5Exception.what()) << std::endl;
        }
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5RequestUsernamePassword)", errorCode.message()) << std::endl;
      }
   }
  );
}

void socks5::Socks5SessionUsernamePassword::WriteSocks5RequestUsernamePassword()
{
}

uint8_t socks5::Socks5SessionUsernamePassword::GetAuthenticationMethod() const
{
  return Socks5RequestHandshake::Method::USERNAME_PASSWORD;
}
