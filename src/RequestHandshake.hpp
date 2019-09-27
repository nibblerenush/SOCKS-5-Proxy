#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  enum Method : uint8_t
  {
    NO_AUTHENTICATION_REQUIRED = 0x00,
    GSSAPI = 0x01,
    USERNAME_PASSWORD = 0x02,
    /*
      From 0x03 to 0x7F IANA
      From 0x80 to 0xFE RESERVED FOR PRIVATE METHODS
    */
    NO_ACCEPTABLE_METHODS = 0xFF
  };

  class RequestHandshake
  {
  public:
    RequestHandshake(const std::vector<uint8_t> & buffer, std::size_t readedLength, Method neededMethod);
    std::string ToString() const;
    Method NeededMethod() const;

  private:
    bool CheckCorrectness() const noexcept;
  
  private:
    std::size_t _readedLength;
    Method _neededMethod;

    uint8_t _ver;
    uint8_t _nmethods;
    std::vector<uint8_t> _methods;
  };
}
