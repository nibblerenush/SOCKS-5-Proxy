#include "ReplySocks.hpp"
#include "RequestSocks.hpp"
#include "Version.hpp"

#include <cstring>
#include <sstream>
#include <typeinfo>

#include <boost/asio.hpp>

#ifdef WIN32
  #include <Windows.h>
#else
  #include <arpa/inet.h>
#endif

namespace socks5
{
  static const std::map<uint8_t, std::string> REP =
  {
    {SUCCEEDED, "SUCCEEDED"},
  };

  static const uint8_t RESERVED_VALUE{0x00};
  static const uint8_t REPLY_BUFFER_SIZE{10};

  ReplySocks::ReplySocks(uint32_t bndAddr, uint16_t bndPort):
  _ver{VER},
  _rep{SUCCEEDED},
  _rsv{RESERVED_VALUE},
  _atyp{IP_V4_ADDRESS},
  _bndAddr{bndAddr},
  _bndPort{bndPort}
  {}

  std::vector<uint8_t> ReplySocks::GenBuff() const
  {
    std::vector<uint8_t> replyBuffer(REPLY_BUFFER_SIZE);
    
    replyBuffer.at(0) = _ver;
    replyBuffer.at(1) = _rep;
    replyBuffer.at(2) = _rsv;
    replyBuffer.at(3) = _atyp;

    uint32_t addr{htonl(_bndAddr)};
    std::memcpy(&replyBuffer.at(4), &addr, sizeof(addr));

    uint16_t port{htons(_bndPort)};
    std::memcpy(&replyBuffer.at(8), &port, sizeof(port));
    
    return replyBuffer;
  }

  std::string ReplySocks::ToString() const
  {
    namespace bai = boost::asio::ip;
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "REP = " << REP.at(_rep) << "; "
      << "RSV = " << (int)_rsv << "; "
      << "ATYP = " << ATYP.at(_atyp) << "; "
      << "BND.ADDR = " << bai::make_address_v4(_bndAddr).to_string() << "; "
      << "BND.PORT = " << _bndPort;

    return result.str();
  }
}
