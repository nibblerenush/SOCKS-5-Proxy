#include "Socks5Server.h"

#include "ErrorPrinter.h"
//#include "Socks5Session.h"

#include <iostream>
#include <memory>
#include <utility>

const uint16_t socks5::Socks5Server::DEFAULT_SERVER_PORT = 1080;
const uint16_t socks5::Socks5Server::DEFAULT_BUFFER_SIZE = 1024;

socks5::Socks5Server::Socks5Server(ba::io_context & ioContext, const bpt::ptree & configurationFile):
  _configurationFile(configurationFile),
  _acceptor(ioContext, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), _configurationFile.get<uint16_t>("server.port", DEFAULT_SERVER_PORT))),
  _sessionIdCounter(0)
{
  Accept();
}

void socks5::Socks5Server::Accept()
{
  /*_acceptor.async_accept
  (
    [this](boost::system::error_code errorCode, ba::ip::tcp::socket acceptSocket)
    {
      if (!errorCode)
      {
        std::make_shared<Socks5SessionBase>(std::move(acceptSocket), _bufferSize, _sessionIdCounter++)->Start();
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionIdCounter, "async_accept", errorCode.message()) << std::endl;
      }
      Accept();
    }
  );*/
}
