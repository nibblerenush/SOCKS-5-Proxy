#include "RequestHmac.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <algorithm>
#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  static const uint16_t MIN_REQUEST_HMAC_SIZE = 3;
  static const uint16_t MAX_REQUEST_HMAC_SIZE = 257;
  
  RequestHmac::RequestHmac(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength{readedLength}
  {
    _ver = buffer.at(0);
    
    _hmacLen = buffer.at(1);
    std::copy(buffer.begin() + 2, buffer.begin() + 2 + _hmacLen, std::back_inserter(_hmac));

    if (!CheckCorrectness())
    {
      throw Exception("Incorrect format of socks5 request username password");
    }

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::string RequestHmac::ToString() const
  {
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "HMACLEN = " << (int)_hmacLen << "; "
      << "HMAC = ";

    for (const auto byte : _hmac)
    {
      result << std::hex << (int)byte << std::dec << ' ';
    }

    return result.str();
  }

  std::vector<uint8_t> RequestHmac::Hmac() const
  {
    return _hmac;
  }

  bool RequestHmac::CheckCorrectness() const noexcept
  {
    if (_ver != LOGIN_HMAC_VER || _readedLength < MIN_REQUEST_HMAC_SIZE || _readedLength > MAX_REQUEST_HMAC_SIZE)
    {
      return false;
    }
    return true;
  }
}
