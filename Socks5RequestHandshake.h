#ifndef _SOCKS_5_REQUEST_HANDSHAKE_H_
#define _SOCKS_5_REQUEST_HANDSHAKE_H_

#include <map>
#include <string>
#include <vector>

namespace socks5
{
  class Socks5RequestHandshake
  {
  public:
    Socks5RequestHandshake(const std::vector<uint8_t> & rawData, std::size_t readedLength);
    std::string ToString() const;
    uint8_t GetVer() const;
    uint8_t GetNmethods() const;
    const std::vector<uint8_t> & GetMethods() const;

  public:
    enum Methods
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
    static const std::map<const uint8_t, const std::string> METHODS;

  private:
    void CheckCorrectness(const std::vector<uint8_t> & rawData, std::size_t readedLength) const;

  private:
    uint8_t _ver;
    uint8_t _nmethods;
    std::vector<uint8_t> _methods;

  private:
    static const uint16_t MIN_REQUEST_HANDSHAKE_SIZE;
    static const uint16_t MAX_REQUEST_HANDSHAKE_SIZE;
  };
}

#endif
