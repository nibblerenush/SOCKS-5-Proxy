#include "Exception.hpp"

namespace socks5
{
  Exception::Exception(const std::string & message):
  std::exception{},
  _message{message}
  {}
  
  const char * Exception::what() const noexcept
  {
    return _message.c_str();
  }
}
