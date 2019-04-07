#ifndef _SOCKS_5_SERVER_H_
#define _SOCKS_5_SERVER_H_

#include <cstdint>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace socks5
{
  namespace ba = boost::asio;
  namespace bpt = boost::property_tree;

  class Socks5Server
  {
  public:
    Socks5Server(ba::io_context & ioContext, const bpt::ptree & configurationFile);
  private:
    void Accept();
  private:
    const bpt::ptree & _configurationFile;
    ba::ip::tcp::acceptor _acceptor;
    int _sessionIdCounter;
  private:
    static const uint16_t DEFAULT_SERVER_PORT;
    static const uint8_t DEFAULT_AUTHENTICATION_METHOD;
  };
}

#endif
