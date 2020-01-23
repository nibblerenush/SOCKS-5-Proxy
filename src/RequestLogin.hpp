#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class RequestLogin
  {
  public:
    RequestLogin(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;
    std::string Login() const;

  private:
    bool CheckCorrectness() const noexcept;

  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _loginLen;
    std::string _login{};
  };
}
