#include "ReplyHandshake.hpp"
#include "Version.hpp"

#include <sstream>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace socks5
{
  ReplyHandshake::ReplyHandshake(Method neededMethod)
  {
    _ver = VER;
    _method = neededMethod;

    BOOST_LOG_TRIVIAL(info) << ToString();
  }
  
  std::vector<uint8_t> ReplyHandshake::GenBuff() const
  {
    std::vector<uint8_t> replyBuffer = {_ver, _method};
    return replyBuffer;
  }
  
  std::string ReplyHandshake::ToString() const
  {
    std::stringstream result;
    
    result
      << typeid(*this).name() << ": "
      << "VER = " << (int)_ver << "; "
      << "METHOD = " << (int)_method;
      
    return result.str();
  }
}
