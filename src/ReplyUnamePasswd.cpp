#include "ReplyUnamePasswd.hpp"
#include "Version.hpp"

#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  ReplyUnamePasswd::ReplyUnamePasswd(uint8_t status)
  {
    _ver = UNAME_PASSWD_VER;
    _status = status;

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::vector<uint8_t> ReplyUnamePasswd::GenBuff() const
  {
    std::vector<uint8_t> replyBuffer = { _ver, _status };
    return replyBuffer;
  }
  
  std::string ReplyUnamePasswd::ToString() const
  {
    std::stringstream result;
    
    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "STATUS = " << (int)_status;
      
    return result.str();
  }
}
