#ifndef _SOCKS_5_SESSION_H_
#define _SOCKS_5_SESSION_H_

#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

namespace socks5
{
  namespace ba = boost::asio;

  class Socks5Session : public std::enable_shared_from_this<Socks5Session>
  {
  public:
    Socks5Session(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);
    void Start();

  private:
    void ReadSocks5Handshake();
    void DoWriteSocks5Handshake();
    void DoReadSocks5Request();
    void DoResolve();
    void DoConnect(ba::ip::tcp::resolver::iterator & resolverIterator);
    void DoWriteSocks5Reply();
    void DoRead(int direction);
    void DoWrite(int direction, std::size_t length);

  private:
    enum { MAX_BUFFER_LENGTH = 1024 };
    enum { REPLY_HANDSHAKE_LENGTH = 2 };

  private:
    ba::ip::tcp::socket _inSocket;
    ba::ip::tcp::socket _outSocket;
    
    std::vector<uint8_t> _requestHandshakeBuff;
    std::vector<uint8_t> _replyHandshakeBuff;
    
    std::vector<uint8_t> _socks5RequesåBuff;
    std::vector<uint8_t> _socks5ReplyBuff;

    std::vector<uint8_t> _inBuffer;
    std::vector<uint8_t> _outBuffer;

    ba::ip::tcp::resolver _resolver;
    int _sessionId;
  };
}

#endif
