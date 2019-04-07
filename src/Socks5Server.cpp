#include "Socks5Server.h"
#include "ErrorPrinter.h"
#include "Socks5Exception.h"
#include "Socks5SessionFactory.h"

#include <iostream>
#include <memory>
#include <utility>

const uint16_t socks5::Socks5Server::DEFAULT_SERVER_PORT = 1080;
const uint8_t socks5::Socks5Server::DEFAULT_AUTHENTICATION_METHOD = 0;

socks5::Socks5Server::Socks5Server(ba::io_context & ioContext, const bpt::ptree & configurationFile):
  _configurationFile(configurationFile),
  _acceptor(ioContext, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), _configurationFile.get<uint16_t>("server.port", DEFAULT_SERVER_PORT))),
  _sessionIdCounter(0)
{
  Socks5SessionFactory::SetType((Socks5RequestHandshake::Method)_configurationFile.get<uint8_t>("server.authentication_method", DEFAULT_AUTHENTICATION_METHOD));
  Accept();
}

void socks5::Socks5Server::Accept()
{
  _acceptor.async_accept
  (
    [this](const boost::system::error_code & errorCode, ba::ip::tcp::socket acceptSocket)
    {
      if (!errorCode)
      {
        try
        {
          Socks5SessionFactory::CreateSession(std::move(acceptSocket), _configurationFile, _sessionIdCounter++)->Start();
        }
        catch (Socks5Exception exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionIdCounter, "async_accept", exception.what()) << std::endl;
        }
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionIdCounter, "async_accept", errorCode.message()) << std::endl;
      }
      Accept();
    }
  );
}
