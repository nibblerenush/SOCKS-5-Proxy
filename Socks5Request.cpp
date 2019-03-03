#include "Socks5Request.h"
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

socks5::Socks5Request::Socks5Request(const std::vector<uint8_t> & rawData)
{
  namespace ba = boost::asio;

  _ver = rawData[0];
  _cmd = rawData[1];
  _rsv = rawData[2];
  _atyp = rawData[3];

  switch (_atyp)
  {
  case IP_V4_ADDRESS:
    _dstAddr = ba::ip::address_v4(GetIpV4Address(rawData)).to_string();
    _dstPort = std::to_string(GetIpV4Port(rawData));
    break;
  case DOMAINNAME:
    _dstAddr = GetDomainName(rawData);
    _dstPort = std::to_string(GetDomainNamePort(rawData));
    break;
  case IP_V6_ADDRESS:
    break;
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

std::string socks5::Socks5Request::GetPort() const
{
  return _dstPort;
}

uint32_t socks5::Socks5Request::GetIpV4Address(const std::vector<uint8_t> & rawData) const
{
  uint32_t * addressBegin = (uint32_t*)&rawData[4];
  return ntohl(*addressBegin);
}

uint16_t socks5::Socks5Request::GetIpV4Port(const std::vector<uint8_t> & rawData) const
{
  uint16_t * portBegin = (uint16_t*)&rawData[8];
  return ntohs(*portBegin);
}

std::string socks5::Socks5Request::GetDomainName(const std::vector<uint8_t> & rawData) const
{
  uint8_t hostLength = rawData[4];
  std::string hostName((char*)&rawData[5], hostLength);
  return hostName;
}

uint16_t socks5::Socks5Request::GetDomainNamePort(const std::vector<uint8_t>& rawData) const
{
  uint8_t hostLength = rawData[4];
  uint16_t * portBegin = (uint16_t*)&rawData[5 + hostLength];
  return ntohs(*portBegin);
}
