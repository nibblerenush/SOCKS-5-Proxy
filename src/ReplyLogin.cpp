#include "ReplyLogin.hpp"
#include "Version.hpp"

#include <random>
#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  static const uint8_t RANDOM_STRING_LENGTH = 128;

  const uint8_t ReplyLogin::SUCCESS = 0;
  const uint8_t ReplyLogin::FAILURE = 1;

  ReplyLogin::ReplyLogin(uint8_t status):
  _ver{LOGIN_HMAC_VER},
  _status{status},
  _randomStringLen{RANDOM_STRING_LENGTH}
  {}
  
  std::vector<uint8_t> ReplyLogin::GenBuff()
  {
    std::vector<uint8_t> replyBuffer = { _ver, _status };
    if (_status == FAILURE)
    {
      return replyBuffer; 
    }
    
    GenRandomString(RANDOM_STRING_LENGTH);
    replyBuffer.push_back(RANDOM_STRING_LENGTH);
    replyBuffer.insert(replyBuffer.end(), _randomString.begin(), _randomString.end());
    return replyBuffer;
  }

  std::vector<uint8_t> ReplyLogin::RandomString()
  {
    return _randomString;
  }
  
  std::string ReplyLogin::ToString() const
  {
    std::stringstream result;
    
    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "STATUS = " << (int)_status << "; "
      << "RANDOM STRING LENGTH = " << (int)_randomStringLen << "; "
      << "RANDOM STRING = ";

    for (const uint8_t byte : _randomString)
    {
      result << std::hex << (int)byte << std::dec << ' ';
    }

    return result.str();
  }

  void ReplyLogin::GenRandomString(uint8_t size)
  {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<uint8_t> distribution(0, 255);

    for (unsigned int i = 0; i < size; ++i)
    {
      _randomString.push_back(distribution(generator));
    }
  }
}
