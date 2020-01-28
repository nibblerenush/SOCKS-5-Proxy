#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class RequestHmac
  {
  public:
    RequestHmac(const std::vector<uint8_t> & buffer, std::size_t readedLength);
    std::string ToString() const;
    std::vector<uint8_t> Hmac() const;

  private:
    bool CheckCorrectness() const noexcept;

  private:
    std::size_t _readedLength;

    uint8_t _ver;
    uint8_t _hmacLen;
    std::vector<uint8_t> _hmac{};
  };
}
