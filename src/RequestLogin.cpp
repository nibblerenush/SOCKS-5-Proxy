#include "RequestLogin.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <algorithm>
#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  static const uint16_t MIN_REQUEST_LOGIN_SIZE = 3;
  static const uint16_t MAX_REQUEST_LOGIN_SIZE = 257;
  
  RequestLogin::RequestLogin(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength{readedLength}
  {
    _ver = buffer.at(0);
    
    _loginLen = buffer.at(1);
    std::copy(buffer.begin() + 2, buffer.begin() + 2 + _loginLen, std::back_inserter(_login));

    if (!CheckCorrectness())
    {
      throw Exception("Incorrect format of socks5 request username password");
    }

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::string RequestLogin::ToString() const
  {
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "LOGINLEN = " << (int)_loginLen << "; "
      << "LOGIN = " << _login;
      
    return result.str();
  }

  std::string RequestLogin::Login() const
  {
    return _login;
  }

  bool RequestLogin::CheckCorrectness() const noexcept
  {
    if (_ver != LOGIN_HMAC_VER || _readedLength < MIN_REQUEST_LOGIN_SIZE || _readedLength > MAX_REQUEST_LOGIN_SIZE)
    {
      return false;
    }
    return true;
  }
}
