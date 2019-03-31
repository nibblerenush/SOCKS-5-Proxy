#include "Socks5Reply.h"
#include "Socks5Version.h"

#include <cstring>
#include <sstream>

const uint8_t socks5::Socks5Reply::RESERVED_VALUE = 0x00;
const uint8_t socks5::Socks5Reply::REPLY_BUFFER_SIZE = 10;

socks5::Socks5Reply::Socks5Reply(uint32_t realRemoteIp, uint16_t realRemotePort)
{
  _ver = Socks5Version::VER;
  _rep = SUCCEEDED;
  _rsv = RESERVED_VALUE;
  _atyp = Socks5Request::IP_V4_ADDRESS;

  // TODO
  // Реализовать DOMAINNAME и IP_V6_ADDRESS
  _realRemoteIp = realRemoteIp;
  _realRemotePort = realRemotePort;
}

std::vector<uint8_t> socks5::Socks5Reply::GenerateReplyBuffer() const
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

std::string socks5::Socks5Reply::ToString() const
{
  // TODO
  // Доделать строковый вывод
  std::stringstream result;

  result
    << "VER: " << (int)_ver << '\n'
    << "REP: " << (int)_rep << '\n';

  return result.str();
}
