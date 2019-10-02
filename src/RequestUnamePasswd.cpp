#include "RequestUnamePasswd.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include <algorithm>
#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  static const uint16_t MIN_REQUEST_UNAME_PASSWD_SIZE = 5;
  static const uint16_t MAX_REQUEST_UNAME_PASSWD_SIZE = 513;
  
  RequestUnamePasswd::RequestUnamePasswd(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength{readedLength}
  {
    _ver = buffer.at(0);
    
    _ulen = buffer.at(1);
    std::copy(buffer.begin() + 2, buffer.begin() + 2 + _ulen, std::back_inserter(_uname));
    
    _plen = buffer.at(2 + _ulen);
    std::copy(buffer.begin() + 3 + _ulen, buffer.begin() + 3 + _ulen + _plen, std::back_inserter(_passwd));
    
    if (!CheckCorrectness())
    {
      throw Exception("Incorrect format of socks5 request username password");
    }

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::string RequestUnamePasswd::ToString() const
  {
    std::stringstream result;

    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "ULEN = " << (int)_ulen << "; "
      << "UNAME = " << _uname << "; "
      << "PLEN = " << (int)_plen << "; "
      << "PASSWD = " << _passwd;
      
    return result.str();
  }

  std::string RequestUnamePasswd::Uname() const
  {
    return _uname;
  }

  std::string RequestUnamePasswd::Passwd() const
  {
    return _passwd;
  }

  bool RequestUnamePasswd::CheckCorrectness() const noexcept
  {
    if (_ver != UNAME_PASSWD_VER || _readedLength < MIN_REQUEST_UNAME_PASSWD_SIZE || _readedLength > MAX_REQUEST_UNAME_PASSWD_SIZE)
    {
      return false;
    }
    return true;
  }
}
