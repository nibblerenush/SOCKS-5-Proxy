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

socks5::Socks5RequestHandshake::Socks5RequestHandshake(const std::vector<uint8_t> & rawData, std::size_t readedLength)
{
  CheckCorrectness(rawData, readedLength);

  _ver = rawData[0];
  _nmethods = rawData[1];

  for (int method_index = 0; method_index < _nmethods; ++method_index)
  {
    _methods.push_back(rawData[2 + method_index]);
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

uint8_t socks5::Socks5RequestHandshake::GetVer() const
{
  return _ver;
}

uint8_t socks5::Socks5RequestHandshake::GetNmethods() const
{
  return _nmethods;
}

const std::vector<uint8_t> & socks5::Socks5RequestHandshake::GetMethods() const
{
  return _methods;
}

void socks5::Socks5RequestHandshake::CheckCorrectness(const std::vector<uint8_t> & rawData, std::size_t readedLength) const
{
  if (rawData[0] != Socks5Version::VER || readedLength < MIN_REQUEST_HANDSHAKE_SIZE || readedLength > MAX_REQUEST_HANDSHAKE_SIZE)
  {
    throw Socks5Exception("Incorrect format of request handshake");
  }
}
