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
  private:
    enum Direction
    {
      RECV_FROM_IN_SEND_TO_OUT = 0x01,
      RECV_FROM_OUT_SEND_TO_IN = 0x02,
      BOTH = 0x03
    };

  public:
    Socks5SessionBase(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId);
    void Start();

  protected:
    void ReadSocks5RequestHandshake();
    void WriteSocks5ReplyHandshake();
    virtual void Authenticate() = 0;
    void ReadSocks5Request();
    void Resolve(std::string _dstAddr, std::string dstPort);
    void Connect(const ba::ip::tcp::resolver::results_type & resultsType);
    void WriteSocks5Reply();
    void Read(Direction direction);
    void Write(Direction direction, std::size_t writeLength);

  protected:
    virtual uint8_t GetAuthenticationMethod() const = 0;

  private:
    void CloseSockets();

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
