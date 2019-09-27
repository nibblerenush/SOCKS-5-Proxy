#include "Reply.hpp"
#include "Request.hpp"
#include "Version.hpp"

#include <cstring>
#include <sstream>
#include <typeinfo>

namespace socks5
{
  static const uint8_t RESERVED_VALUE {0x00};
  static const uint8_t REPLY_BUFFER_SIZE {10};

  Reply::Reply(uint32_t realRemoteIp, uint16_t realRemotePort)
  {
    _ver = VER;
    _rep = SUCCEEDED;
    _rsv = RESERVED_VALUE;
    _atyp = IP_V4_ADDRESS;

    // TODO
    // Реализовать DOMAINNAME и IP_V6_ADDRESS
    _realRemoteIp = realRemoteIp;
    _realRemotePort = realRemotePort;
  }

  std::vector<uint8_t> Reply::GenBuff() const
  {
    std::vector<uint8_t> replyBuffer(REPLY_BUFFER_SIZE);
  
    replyBuffer[0] = _ver;
    replyBuffer[1] = _rep;
    replyBuffer[2] = _rsv;
    replyBuffer[3] = _atyp;

    std::memcpy(&replyBuffer[4], &_realRemoteIp, sizeof(_realRemoteIp));
    std::memcpy(&replyBuffer[8], &_realRemotePort, sizeof(_realRemotePort));

    return replyBuffer;
  }

  std::string Reply::ToString() const
  {
    // TODO
    // Доделать строковый вывод
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "REP = " << (int)_rep << "; "
      << "RSV = " << (int)_rsv << "; "
      << "ATYP = " << (int)_atyp;

    return result.str();
  }

}