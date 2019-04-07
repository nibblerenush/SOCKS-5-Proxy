#include "Socks5SessionUsernamePassword.h"
#include "ErrorPrinter.h"
#include "Socks5Exception.h"
#include "Socks5RequestHandshake.h"
#include "Socks5RequestUsernamePassword.h"
#include "Socks5ReplyUsernamePassword.h"

#include <iostream>

socks5::Socks5SessionUsernamePassword::Socks5SessionUsernamePassword(
  ba::ip::tcp::socket && socket,
  const std::string & username,
  const std::string & password,
  uint16_t bufferSize,
  int sessionId):
  Socks5SessionBase(std::move(socket), bufferSize, sessionId),
  _username(username),
  _password(password),
  _socks5RequestUsernamePasswordBuff(bufferSize)
{}

void socks5::Socks5SessionUsernamePassword::Authenticate()
{
  ReadSocks5RequestUsernamePassword();
}

uint8_t socks5::Socks5SessionUsernamePassword::GetAuthenticationMethod() const
{
  return Socks5RequestHandshake::Method::USERNAME_PASSWORD;
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
          Socks5RequestUsernamePassword socks5RequestUsernamePassword(_socks5RequestUsernamePasswordBuff, readedLength);
          //std::cout << socks5RequestUsernamePassword.ToString() << std::endl;

          if (socks5RequestUsernamePassword.GetUsername() == _username &&
              socks5RequestUsernamePassword.GetPassword() == _password)
          {
            Socks5ReplyUsernamePassword socks5ReplyUsernamePassword(SUCCESS);
            //std::cout << socks5ReplyUsernamePassword.ToString() << std::endl;
            _socks5ReplyUsernamePasswordBuff = socks5ReplyUsernamePassword.GenerateReplyUsernamePasswordBuffer();
            WriteSocks5RequestUsernamePasswordSuccess();
          }
          else
          {
            Socks5ReplyUsernamePassword socks5ReplyUsernamePassword(FAILURE);
            //std::cout << socks5ReplyUsernamePassword.ToString() << std::endl;
            _socks5ReplyUsernamePasswordBuff = socks5ReplyUsernamePassword.GenerateReplyUsernamePasswordBuffer();
            WriteSocks5RequestUsernamePasswordFailure();
          }
        }
        catch (Socks5Exception socks5Exception)
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

void socks5::Socks5SessionUsernamePassword::WriteSocks5RequestUsernamePasswordSuccess()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_socks5ReplyUsernamePasswordBuff, _socks5ReplyUsernamePasswordBuff.size()),
    [this, self](const boost::system::error_code & errorCode, std::size_t writedLength)
    {
      if (!errorCode)
      {
        ReadSocks5Request();
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(WriteSocks5RequestUsernamePasswordSuccess)", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionUsernamePassword::WriteSocks5RequestUsernamePasswordFailure()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_socks5ReplyUsernamePasswordBuff, _socks5ReplyUsernamePasswordBuff.size()),
    [this, self](const boost::system::error_code & errorCode, std::size_t writedLength)
    {
      if (errorCode)
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(WriteSocks5RequestUsernamePasswordFailure)", errorCode.message()) << std::endl;
      }
    }
  );
}
