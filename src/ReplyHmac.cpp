#include "ReplyHmac.hpp"
#include "Version.hpp"

#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  const uint8_t ReplyHmac::SUCCESS = 0;
  const uint8_t ReplyHmac::FAILURE = 1;

  ReplyHmac::ReplyHmac(uint8_t status)
  {
    _ver = UNAME_PASSWD_VER;
    _status = status;

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::vector<uint8_t> ReplyHmac::GenBuff() const
  {
    std::vector<uint8_t> replyBuffer = { _ver, _status };
    return replyBuffer;
  }
  
  std::string ReplyHmac::ToString() const
  {
    std::stringstream result;
    
    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "STATUS = " << (int)_status;
      
    return result.str();
  }
}
