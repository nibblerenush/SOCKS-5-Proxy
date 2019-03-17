#include "Socks5RequestHandshake.h"
#include "Socks5Exception.h"
#include "Socks5Version.h"

#include <sstream>

const std::map<const uint8_t, const std::string> socks5::Socks5RequestHandshake::METHODS =
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
const uint16_t socks5::Socks5RequestHandshake::MIN_REQUEST_HANDSHAKE_SIZE = 3;
const uint16_t socks5::Socks5RequestHandshake::MAX_REQUEST_HANDSHAKE_SIZE = 257;

socks5::Socks5RequestHandshake::Socks5RequestHandshake(const std::vector<uint8_t> & buffer, std::size_t readedLength, Method neededMethod):
  _readedLength(readedLength),
  _neededMethod(neededMethod)
{
  _ver = buffer[0];
  _nmethods = buffer[1];
  
  for (int method_index = 0; method_index < _nmethods; ++method_index)
  {
    _methods.push_back(buffer[2 + method_index]);
  }
  
  if (!CheckCorrectness())
  {
    throw Socks5Exception("Incorrect format of socks5 request handshake");
  }
}

std::string socks5::Socks5RequestHandshake::ToString() const
{
  std::stringstream result;

  result
    << "VER: " << (int)_ver << '\n'
    << "NMETHODS: " << (int)_nmethods << '\n'
    << "METHODS: ";

  for (int i = 0; i < _methods.size(); ++i)
  {
    result << METHODS.at(_methods.at(i)) << ' ';
  }

  return result.str();
}

socks5::Socks5RequestHandshake::Method socks5::Socks5RequestHandshake::GetNeededMethod() const
{
  return _neededMethod;
}

bool socks5::Socks5RequestHandshake::CheckCorrectness() const noexcept
{
  if (_ver != Socks5Version::VER || _readedLength < MIN_REQUEST_HANDSHAKE_SIZE || _readedLength > MAX_REQUEST_HANDSHAKE_SIZE)
  {
    return false;
  }

  for (int i = 0; i < _methods.size(); ++i)
  {
    if (_methods[i] == _neededMethod)
    {
      return true;
    }
  }

  return false;
}
