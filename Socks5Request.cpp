#include "Socks5Request.h"
#include "Socks5Exception.h"
#include "Socks5Version.h"

#include <boost/asio.hpp>
#include <sstream>

#ifdef WIN32
  #include <Windows.h>
#else
  #include <arpa/inet.h>
#endif

const std::map<const uint8_t, const std::string> socks5::Socks5Request::Socks5Request::CMD =
{
  {CONNECT, "CONNECT"},
  {BIND, "BIND"},
  {UDP_ASSOCIATE, "UDP ASSOCIATE"}
};

const std::map<const uint8_t, const std::string> socks5::Socks5Request::Socks5Request::ATYP =
{
  {IP_V4_ADDRESS, "IP V4 address"},
  {DOMAINNAME, "DOMAINNAME"},
  {IP_V6_ADDRESS, "IP V6 address"}
};

const uint8_t socks5::Socks5Request::MIN_REQUEST_SIZE = 5;
const uint8_t socks5::Socks5Request::RESERVED_VALUE = 0x00;

socks5::Socks5Request::Socks5Request(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength(readedLength)
{
  namespace ba = boost::asio;

  _ver = buffer[0];
  _cmd = buffer[1];
  _rsv = buffer[2];
  _atyp = buffer[3];

  switch (_atyp)
  {
  case IP_V4_ADDRESS:
    _dstAddr = ba::ip::address_v4(GetIpV4Address(buffer)).to_string();
    _dstPort = std::to_string(GetIpV4AddressPort(buffer));
    break;
  case DOMAINNAME:
    _dstAddr = GetDomainName(buffer);
    _dstPort = std::to_string(GetDomainNamePort(buffer));
    break;
  case IP_V6_ADDRESS:
    // TODO
    // Реализовать обработку IP_V6_ADDRESS
    break;
  }

  if (!CheckCorrectness())
  {
    throw Socks5Exception("Incorrect format of socks5 request");
  }
}

std::string socks5::Socks5Request::ToString() const
{
  std::stringstream result;

  result
    << "VER: " << (int)_ver << '\n'
    << "CMD: " << CMD.at(_cmd) << '\n'
    << "RSV: " << (int)_rsv << '\n'
    << "ATYP: " << ATYP.at(_atyp) << '\n'
    << _dstAddr << '\n'
    << _dstPort << '\n';

  return result.str();
}

std::string socks5::Socks5Request::GetDstAddr() const
{
  return _dstAddr;
}

std::string socks5::Socks5Request::GetDstPort() const
{
  return _dstPort;
}

bool socks5::Socks5Request::CheckCorrectness() const noexcept
{
  if (_ver != Socks5Version::VER || _cmd != CONNECT || _rsv != RESERVED_VALUE || _readedLength < MIN_REQUEST_SIZE)
  {
    return false;
  }
}

uint32_t socks5::Socks5Request::GetIpV4Address(const std::vector<uint8_t> & buffer) const
{
  uint32_t * addressBegin = (uint32_t*)&buffer[4];
  return ::ntohl(*addressBegin);
}

uint16_t socks5::Socks5Request::GetIpV4AddressPort(const std::vector<uint8_t> & buffer) const
{
  uint16_t * portBegin = (uint16_t*)&buffer[8];
  return ::ntohs(*portBegin);
}

std::string socks5::Socks5Request::GetDomainName(const std::vector<uint8_t> & buffer) const
{
  uint8_t hostLength = buffer[4];
  std::string hostName((char*)&buffer[5], hostLength);
  return hostName;
}

uint16_t socks5::Socks5Request::GetDomainNamePort(const std::vector<uint8_t>& buffer) const
{
  uint8_t hostLength = buffer[4];
  uint16_t * portBegin = (uint16_t*)&buffer[5 + hostLength];
  return ::ntohs(*portBegin);
}
