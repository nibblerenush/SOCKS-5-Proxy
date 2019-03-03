#ifndef _SOCKS_5_EXCEPTION_H_
#define _SOCKS_5_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace socks5
{
  class Socks5Exception : public std::exception
  {
  public:
    Socks5Exception(const std::string & message);
    virtual const char * what() const override;
  private:
    std::string _message;
  };
}

#endif
