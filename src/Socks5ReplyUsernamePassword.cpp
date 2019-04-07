#include "Socks5ReplyUsernamePassword.h"
#include "Socks5Version.h"

#include <sstream>

socks5::Socks5ReplyUsernamePassword::Socks5ReplyUsernamePassword(uint8_t status)
{
  _ver = Socks5Version::USERNAME_PASSWORD_VER;
  _status = status;
}

std::vector<uint8_t> socks5::Socks5ReplyUsernamePassword::GenerateReplyUsernamePasswordBuffer() const
{
  std::vector<uint8_t> replyBuffer = { _ver, _status };
  return replyBuffer;
}

std::string socks5::Socks5ReplyUsernamePassword::ToString() const
{
  std::stringstream result;
  result
    << "VER: " << (int)_ver << '\n'
    << "STATUS: " << (int)_status << '\n';
  return result.str();
}
