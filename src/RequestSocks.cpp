#include "RequestSocks.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <algorithm>
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
  static const std::map<uint8_t, std::string> CMD =
  {
    {CONNECT, "CONNECT"},
    {BIND, "BIND"},
    {UDP_ASSOCIATE, "UDP ASSOCIATE"}
  };
  
  static const uint8_t MIN_REQUEST_SIZE = 5;
  static const uint8_t RESERVED_VALUE = 0x00;
  
  RequestSocks::RequestSocks(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength{readedLength}
  {
    _ver = buffer.at(0);
    _cmd = buffer.at(1);
    _rsv = buffer.at(2);
    _atyp = buffer.at(3);
    
    switch (_atyp)
    {
      case IP_V4_ADDRESS:
        _dstAddr = IpV4Address(buffer);
        _dstPort = std::to_string(IpV4AddressPort(buffer));
        break;
      case DOMAINNAME:
        _dstAddr = DomainName(buffer);
        _dstPort = std::to_string(DomainNamePort(buffer));
        break;
      case IP_V6_ADDRESS:
        _dstAddr = IpV6Address(buffer);
        _dstPort = std::to_string(IpV6AddressPort(buffer));
        break;
    }
    
    if (!CheckCorrectness())
    {
      throw Exception("Incorrect format of socks5 request");
    }
  }
  
  std::string RequestSocks::ToString() const
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

  std::string RequestSocks::DstAddr() const
  {
    return _dstAddr;
  }

  std::string RequestSocks::DstPort() const
  {
    return _dstPort;
  }

  bool RequestSocks::CheckCorrectness() const noexcept
  {
    if (_ver != VER || _cmd != CONNECT || _rsv != RESERVED_VALUE || _readedLength < MIN_REQUEST_SIZE)
    {
      return false;
    }
    return true;
  }

  std::string RequestSocks::IpV4Address(const std::vector<uint8_t> & buffer) const
  {
    namespace bai = boost::asio::ip;
    bai::address_v4::bytes_type addressBytes;
    std::copy_n(buffer.begin() + 4, addressBytes.size(), addressBytes.begin());
    return bai::make_address_v4(addressBytes).to_string();
  }
  
  uint16_t RequestSocks::IpV4AddressPort(const std::vector<uint8_t> & buffer) const
  {
    uint16_t * portBegin {(uint16_t*)&buffer.at(8)};
    return ntohs(*portBegin);
  }

  std::string RequestSocks::DomainName(const std::vector<uint8_t> & buffer) const
  {
    uint8_t hostLength {buffer.at(4)};
    std::string hostName((char*)&buffer.at(5), hostLength);
    return hostName;
  }

  uint16_t RequestSocks::DomainNamePort(const std::vector<uint8_t> & buffer) const
  {
    uint8_t hostLength {buffer[4]};
    uint16_t * portBegin {(uint16_t*)&buffer.at(5 + hostLength)};
    return ntohs(*portBegin);
  }

  std::string RequestSocks::IpV6Address(const std::vector<uint8_t> & buffer) const
  {
    namespace bai = boost::asio::ip;
    bai::address_v6::bytes_type addressBytes;
    std::copy_n(buffer.begin() + 4, addressBytes.size(), addressBytes.begin());
    return bai::make_address_v6(addressBytes).to_string();
  }
  
  uint16_t RequestSocks::IpV6AddressPort(const std::vector<uint8_t> & buffer) const
  {
    uint16_t * portBegin {(uint16_t*)&buffer.at(20)};
    return ntohs(*portBegin);
  }
}
