#pragma once

#include <stdexcept>
#include <string>

namespace socks5
{
  class Exception: public std::exception
  {
  public:
    Exception(const std::string & message);
    virtual const char * what() const noexcept override;
  private:
    std::string _message;
  };
}
