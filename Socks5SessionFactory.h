#ifndef _SOCKS_5_SESSION_FACTORY_H_
#define _SOCKS_5_SESSION_FACTORY_H_

#include <boost/asio.hpp>

#include "Socks5SessionBase.h"
#include "Socks5RequestHandshake.h"

#include <cstdint>
#include <memory>

namespace socks5
{
  namespace ba = boost::asio;

  class Socks5SessionFactory
  {
  public:
    static void SetType(Socks5RequestHandshake::Method Type);
    static std::shared_ptr<Socks5SessionBase> CreateSession(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);
  private:
    static Socks5RequestHandshake::Method _Type;
  };
}

#endif
