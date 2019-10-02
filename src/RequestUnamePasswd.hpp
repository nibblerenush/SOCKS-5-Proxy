#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class RequestUnamePasswd
  {
  public:
    RequestUnamePasswd(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;
    std::string Uname() const;
    std::string Passwd() const;

  private:
    bool CheckCorrectness() const noexcept;

  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _ulen;
    std::string _uname{};
    uint8_t _plen;
    std::string _passwd{};
  };
}
