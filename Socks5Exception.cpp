#include "Socks5Exception.h"

socks5::Socks5Exception::Socks5Exception(const std::string & message):
std::exception(),
_message(message)
{}

const char * socks5::Socks5Exception::what() const
{
  return _message.c_str();
}
