#ifndef _SOCKS_5_SESSION_FACTORY_H_
#define _SOCKS_5_SESSION_FACTORY_H_

#include "Socks5SessionBase.h"
#include "Socks5RequestHandshake.h"

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <cstdint>
#include <memory>

namespace socks5
{
  namespace ba = boost::asio;
  namespace bpt = boost::property_tree;

  class Socks5SessionFactory
  {
  public:
    static void SetType(Socks5RequestHandshake::Method Type);
    static std::shared_ptr<Socks5SessionBase> CreateSession(
      ba::ip::tcp::socket && socket,
      const bpt::ptree & configurationFile,
      int sessionId);
  private:
    static Socks5RequestHandshake::Method _Type;
    static const uint16_t DEFAULT_BUFFER_SIZE;
  };
}

#endif
