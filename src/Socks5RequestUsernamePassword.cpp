#include "Socks5RequestUsernamePassword.h"
#include "Socks5Exception.h"
#include "Socks5Version.h"

#include <sstream>

socks5::Socks5RequestUsernamePassword::Socks5RequestUsernamePassword(const std::vector<uint8_t> & buffer, std::size_t readedLength):
  _readedLength(readedLength)
{
  _ver = buffer[0];
  _ulen = buffer[1];
  _uname.resize(_ulen);
  for (unsigned int uname_index = 0; uname_index < _ulen; ++uname_index)
  {
    _uname[uname_index] = buffer[2 + uname_index];
  }
  _plen = buffer[2 + _ulen];
  _passwd.resize(_plen);
  for (unsigned int passwd_index = 0; passwd_index < _plen; ++passwd_index)
  {
    _passwd[passwd_index] = buffer[3 + _ulen + passwd_index];
  }
  
  /*if (!CheckCorrectness())
  {
    throw Socks5Exception("Incorrect format of socks5 request username password");
  }*/
}

std::string socks5::Socks5RequestUsernamePassword::ToString() const
{
  std::stringstream result;
  result
    << "VER: " << (int)_ver << '\n'
    << "ULEN: " << (int)_ulen << '\n'
    << "UNAME: " << _uname << '\n'
    << "PLEN: " << (int)_plen << '\n'
    << "PASSWD: " << _passwd;
  return result.str();
}




