#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace socks5
{
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

  class Request
  {
  public:
    Request(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;
    std::string DstAddr() const;
    std::string DstPort() const;
    
  private:
    bool CheckCorrectness() const noexcept;
    uint32_t IpV4Address(const std::vector<uint8_t> & buffer) const;
    uint16_t IpV4AddressPort(const std::vector<uint8_t> & buffer) const;
    std::string DomainName(const std::vector<uint8_t> & buffer) const;
    uint16_t DomainNamePort(const std::vector<uint8_t> & buffer) const;
    
  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _cmd;
    uint8_t _rsv;
    uint8_t _atyp;
    std::string _dstAddr;
    std::string _dstPort;
  };
}
