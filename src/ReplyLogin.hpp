#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace socks5
{
  class ReplyLogin
  {
  public:
    ReplyLogin(uint8_t status);
    std::vector<uint8_t> GenBuff();
    std::vector<uint8_t> RandomString();
    std::string ToString() const;

  public:
    static const uint8_t SUCCESS;
    static const uint8_t FAILURE;

  private:
    void GenRandomString(uint8_t size);
    
  private:
    uint8_t _ver;
    uint8_t _status;
    uint8_t _randomStringLen;
    std::vector<uint8_t> _randomString;
  };
}
