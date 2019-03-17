#ifndef _SOCKS_5_REQUEST_H_
#define _SOCKS_5_REQUEST_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace socks5
{
  class Socks5Request
  {
  public:
    enum Cmd
    {
      CONNECT = 0x01,
      BIND = 0x02,
      UDP_ASSOCIATE = 0x03
    };
    enum Atyp
    {
      IP_V4_ADDRESS = 0x01,
      DOMAINNAME = 0x03,
      IP_V6_ADDRESS = 0x04
    };

  public:
    Socks5Request(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;
    std::string GetDstAddr() const;
    std::string GetDstPort() const;

  private:
    bool CheckCorrectness() const noexcept;

    uint32_t GetIpV4Address(const std::vector<uint8_t> & buffer) const;
    uint16_t GetIpV4AddressPort(const std::vector<uint8_t> & buffer) const;

    std::string GetDomainName(const std::vector<uint8_t> & buffer) const;
    uint16_t GetDomainNamePort(const std::vector<uint8_t> & buffer) const;

  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _cmd;
    uint8_t _rsv;
    uint8_t _atyp;
    std::string _dstAddr;
    std::string _dstPort;

  private:
    static const std::map<const uint8_t, const std::string> CMD;
    static const std::map<const uint8_t, const std::string> ATYP;
    static const uint8_t MIN_REQUEST_SIZE;
    static const uint8_t RESERVED_VALUE;
;  };
}

#endif
