#ifndef _SOCKS_5_SESSION_H_
#define _SOCKS_5_SESSION_H_

#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

namespace socks5
{
  namespace ba = boost::asio;
  
  class Socks5SessionBase: public std::enable_shared_from_this<Socks5SessionBase>
  {
  public:
    Socks5SessionBase(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);
    void Start();
  
  protected:
    void ReadSocks5RequestHandshake();
    void WriteSocks5ReplyHandshake();
    virtual void Authenticate() = 0;
    void ReadSocks5Request();
    /*void Resolve();
    void Connect(ba::ip::tcp::resolver::iterator & resolverIterator);
    void WriteSocks5Reply();
    void Read(int direction);
    void Write(int direction, std::size_t length);*/
  
  protected:
    virtual uint8_t GetAuthenticationMethod() const = 0;

  protected:
    ba::ip::tcp::socket _inSocket;
    ba::ip::tcp::socket _outSocket;

    std::vector<uint8_t> _socks5RequestHandshakeBuff;
    std::vector<uint8_t> _socks5ReplyHandshakeBuff;

    std::vector<uint8_t> _socks5RequestBuff;
    std::vector<uint8_t> _socks5ReplyBuff;

    std::vector<uint8_t> _inBuffer;
    std::vector<uint8_t> _outBuffer;

    ba::ip::tcp::resolver _resolver;
    int _sessionId;
  };
}

#endif
