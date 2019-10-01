#include "RequestHandshake.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <algorithm>
#include <map>
#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  static const std::map<uint8_t, std::string> METHODS =
  {
    {NO_AUTHENTICATION_REQUIRED, "NO AUTHENTICATION REQUIRED"},
    {GSSAPI, "GSSAPI"},
    {USERNAME_PASSWORD, "USERNAME/PASSWORD"},
    /*
      From 0x03 to 0x7F IANA ASSIGNED
      From 0x80 to 0xFE RESERVED FOR PRIVATE METHODS
    */
    {NO_ACCEPTABLE_METHODS, "NO ACCEPTABLE METHODS"}
  };
  
  static const uint16_t MIN_REQUEST_HANDSHAKE_SIZE = 3;
  static const uint16_t MAX_REQUEST_HANDSHAKE_SIZE = 257;
  
  RequestHandshake::RequestHandshake(const std::vector<uint8_t> & buffer, std::size_t readedLength, Method neededMethod):
  _readedLength{readedLength},
  _neededMethod{neededMethod}
  {
    _ver = buffer.at(0);
    _nmethods = buffer.at(1);
    
    for (int method_index{0}; method_index < _nmethods; ++method_index)
    {
      _methods.push_back(buffer.at(2 + method_index));
    }
    
    if (!CheckCorrectness())
    {
      throw Exception("Incorrect format of socks5 request handshake");
    }

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::string RequestHandshake::ToString() const
  {
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "NMETHODS = " << (int)_nmethods << "; "
      << "METHODS = ";

    for (std::size_t i{0}; i < _methods.size(); ++i)
    {
      result << METHODS.at(_methods.at(i)) << ' ';
    }

    return result.str();
  }

  Method RequestHandshake::NeededMethod() const
  {
    return _neededMethod;
  }

  bool RequestHandshake::CheckCorrectness() const noexcept
  {
    if (_ver != VER || _readedLength < MIN_REQUEST_HANDSHAKE_SIZE || _readedLength > MAX_REQUEST_HANDSHAKE_SIZE)
    {
      return false;
    }
    return std::find(_methods.begin(), _methods.end(), _neededMethod) != _methods.end() ? true : false;
  }
}
