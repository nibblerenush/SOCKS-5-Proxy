#ifndef _SOCKS_5_REQUEST_USERNAME_PASSWORD_
#define _SOCKS_5_REQUEST_USERNAME_PASSWORD_

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class Socks5RequestUsernamePassword
  {
  public:
    Socks5RequestUsernamePassword(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;

  private:
    bool CheckCorrectness() const noexcept;

  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _ulen;
    std::string _uname;
    uint8_t _plen;
    std::string _passwd;

  private:
    static const uint16_t MIN_REQUEST_USERNAME_PASSWORD_SIZE;
    static const uint16_t MAX_REQUEST_USERNAME_PASSWORD_SIZE;
  };
}

#endif
