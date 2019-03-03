#include "Socks5Reply.h"

socks5::Socks5Reply::Socks5Reply(uint32_t realRemoteIp, uint16_t realRemotePort)
{
  _ver = 0x05;
  _rep = SUCCEEDED;
  _rsv = 0x00;
  _atyp = 0x01;

  _realRemoteIp = realRemoteIp;
  _realRemotePort = realRemotePort;
}

std::vector<uint8_t> socks5::Socks5Reply::GenerateReplyBuffer() const
{
  std::vector<uint8_t> replyBuffer(10); 
  
  replyBuffer[0] = _ver;
  replyBuffer[1] = _rep;
  replyBuffer[2] = _rsv;
  replyBuffer[3] = _atyp;

  std::memcpy(&replyBuffer[4], &_realRemoteIp, 4);
  std::memcpy(&replyBuffer[8], &_realRemotePort, 2);
  
  return replyBuffer;
}
