#include "Request.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <boost/asio.hpp>
#include <sstream>
#include <typeinfo>

#ifdef WIN32
  #include <Windows.h>
#else
  #include <arpa/inet.h>
#endif

namespace socks5
{
  static const std::map<uint8_t, std::string> CMD =
  {
    {CONNECT, "CONNECT"},
    {BIND, "BIND"},
    {UDP_ASSOCIATE, "UDP ASSOCIATE"}
  };

  static const std::map<uint8_t, std::string> ATYP 
  {
    {IP_V4_ADDRESS, "IP V4 address"},
    {DOMAINNAME, "DOMAINNAME"},
    {IP_V6_ADDRESS, "IP V6 address"}
  };
  
  static const uint8_t MIN_REQUEST_SIZE = 5;
  static const uint8_t RESERVED_VALUE = 0x00;
  
  Request::Request(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength{readedLength}
  {
    _ver = buffer.at(0);
    _cmd = buffer.at(1);
    _rsv = buffer.at(2);
    _atyp = buffer.at(3);
    
    switch (_atyp)
    {
      case IP_V4_ADDRESS:
        _dstAddr = boost::asio::ip::address_v4(IpV4Address(buffer)).to_string();
        _dstPort = std::to_string(IpV4AddressPort(buffer));
        break;
      case DOMAINNAME:
        _dstAddr = DomainName(buffer);
        _dstPort = std::to_string(DomainNamePort(buffer));
        break;
      case IP_V6_ADDRESS:
        // TODO
        // IP_V6_ADDRESS
        break;
    }
    
    if (!CheckCorrectness())
    {
      //throw Socks5Exception("Incorrect format of socks5 request");
    }
  }
  
  std::string Request::ToString() const
  {
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "CMD = " << CMD.at(_cmd) << "; "
      << "RSV = " << (int)_rsv << "; "
      << "ATYP = " << ATYP.at(_atyp) << "; "
      << "DST.ADDR = " << _dstAddr << "; "
      << "DST.PORT = " << _dstPort;

    return result.str();
  }

  std::string Request::DstAddr() const
  {
    return _dstAddr;
  }

  std::string Request::DstPort() const
  {
    return _dstPort;
  }

  bool Request::CheckCorrectness() const noexcept
  {
    if (_ver != VER || _cmd != CONNECT || _rsv != RESERVED_VALUE || _readedLength < MIN_REQUEST_SIZE)
    {
      return false;
    }
    return true;
  }

  uint32_t Request::IpV4Address(const std::vector<uint8_t> & buffer) const
  {
    uint32_t * addressBegin {(uint32_t*)&buffer.at(4)};
    return ::ntohl(*addressBegin);
  }

  uint16_t Request::IpV4AddressPort(const std::vector<uint8_t> & buffer) const
  {
    uint16_t * portBegin {(uint16_t*)&buffer.at(8)};
    return ::ntohs(*portBegin);
  }

  std::string Request::DomainName(const std::vector<uint8_t> & buffer) const
  {
    uint8_t hostLength {buffer.at(4)};
    std::string hostName((char*)&buffer.at(5), hostLength);
    return hostName;
  }

  uint16_t Request::DomainNamePort(const std::vector<uint8_t> & buffer) const
  {
    uint8_t hostLength {buffer[4]};
    uint16_t * portBegin {(uint16_t*)&buffer.at(5 + hostLength)};
    return ::ntohs(*portBegin);
  }
}
