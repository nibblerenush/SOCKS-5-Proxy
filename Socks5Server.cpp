#include "Socks5Server.h"

#include "ErrorPrinter.h"
#include "Socks5Session.h"

#include <iostream>
#include <memory>
#include <utility>

socks5::Socks5Server::Socks5Server(ba::io_context & ioContext, uint16_t serverPort, uint16_t bufferSize):
_acceptor(ioContext, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), serverPort)),
_bufferSize(bufferSize),
_sessionIdCounter(0)
{
  Accept();
}

void socks5::Socks5Server::Accept()
{
  _acceptor.async_accept
  (
    [this](boost::system::error_code errorCode, ba::ip::tcp::socket acceptSocket)
    {
      if (!errorCode)
      {
        std::make_shared<Socks5Session>(std::move(acceptSocket), _bufferSize, _sessionIdCounter++)->Start();
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionIdCounter, "async_accept", errorCode.message()) << std::endl;
      }
      Accept();
    }
  );
}
