#include "Socks5ReplyHandshake.h"
#include "Socks5Version.h"

#include <sstream>

socks5::Socks5ReplyHandshake::Socks5ReplyHandshake(const Socks5RequestHandshake & socks5RequestHandshake)
{
  _ver = Socks5Version::VER;
  _method = socks5RequestHandshake.GetNeededMethod();
}

std::vector<uint8_t> socks5::Socks5ReplyHandshake::GenerateReplyHandshakeBuffer() const
{
  std::vector<uint8_t> replyBuffer = {_ver, _method};
  return replyBuffer;
}

std::string socks5::Socks5ReplyHandshake::ToString() const
{
  std::stringstream result;

  result
    << "VER: " << (int)_ver << '\n'
    << "METHOD: " << (int)_method << '\n';

  return result.str();
}
