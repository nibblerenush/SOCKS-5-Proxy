#pragma once

#include <map>
#include <vector>
#include <string>

namespace socks5
{
  class Socks5Request
  {
  public:
    Socks5Request(const std::vector<uint8_t> & rawData);
    std::string ToString() const;
    std::string GetDstAddr() const;
    std::string GetPort() const;

  public:
    enum Cmd
    {
      CONNECT = 0x01,
      BIND = 0x02,
      UDP_ASSOCIATE = 0x03
    };
    static const std::map<const uint8_t, const std::string> CMD;
    enum Atyp
    {
      IP_V4_ADDRESS = 0x01,
      DOMAINNAME = 0x03,
      IP_V6_ADDRESS = 0x04
    };
    static const std::map<const uint8_t, const std::string> ATYP;

  private:
    uint8_t _ver;
    uint8_t _cmd;
    uint8_t _rsv;
    uint8_t _atyp;
    std::string _dstAddr;
    std::string _dstPort;

  private:
    uint32_t GetIpV4Address(const std::vector<uint8_t> & rawData) const;
    uint16_t GetIpV4Port(const std::vector<uint8_t> & rawData) const;

    std::string GetDomainName(const std::vector<uint8_t> & rawData) const;
    uint16_t GetDomainNamePort(const std::vector<uint8_t> & rawData) const;
  };
}
