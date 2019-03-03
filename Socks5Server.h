#ifndef _SOCKS_5_SERVER_H_
#define _SOCKS_5_SERVER_H_

#include <cstdint>
#include <boost/asio.hpp>

namespace socks5
{
  namespace ba = boost::asio;

  class Socks5Server
  {
  public:
    Socks5Server(ba::io_context & ioContext, uint16_t serverPort, uint16_t bufferSize);
  private:
    void Accept();
  private:
    ba::ip::tcp::acceptor _acceptor;
    const uint16_t _bufferSize;
    int _sessionIdCounter;
  };
}

#endif
