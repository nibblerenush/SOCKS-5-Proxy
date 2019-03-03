#include "Socks5Session.h"
#include "ErrorPrinter.h"
#include "Socks5Exception.h"

#include <iostream>

#include "Socks5RequestHandshake.h"
#include "Socks5ReplyHandshake.h"

#include "Socks5Request.h"
#include "Socks5Reply.h"

socks5::Socks5Session::Socks5Session(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId):
_inSocket(std::move(socket)),
_outSocket(_inSocket.get_io_context()),
_requestHandshakeBuff(bufferSize),
_socks5RequesåBuff(bufferSize),
_inBuffer(bufferSize),
_outBuffer(bufferSize),
_resolver(_inSocket.get_io_context()),
_sessionId(sessionId)
{}

void socks5::Socks5Session::Start()
{
  ReadSocks5Handshake();
}

void socks5::Socks5Session::ReadSocks5Handshake()
{
  auto self(shared_from_this());
  _inSocket.async_receive
  (
    ba::buffer(_requestHandshakeBuff, _requestHandshakeBuff.size()),
    [this, self](boost::system::error_code errorCode, std::size_t readedLength)
    {
      if (!errorCode)
      {
        try
        {
          socks5::Socks5RequestHandshake socks5RequestHandshake(_requestHandshakeBuff, readedLength);
          //std::cout << socks5RequestHandshake.ToString() << std::endl;

          // Çäåñü íåîáõîäèì êîä âûáîðà ìåòîäà àóòåíòèôèêàöèè

          socks5::Socks5ReplyHandshake socks5ReplyHandshake(socks5RequestHandshake);
          //std::cout << socks5ReplyHandshake.ToString();

          _replyHandshakeBuff = socks5ReplyHandshake.GenerateReplyHandshakeBuffer();
          DoWriteSocks5Handshake();
        }
        catch (socks5::Socks5Exception socks5Exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5Handshake)", socks5Exception.what()) << std::endl;
          return;
        }
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5Handshake)", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5Session::DoWriteSocks5Handshake()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_replyHandshakeBuff, REPLY_HANDSHAKE_LENGTH),
    [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        DoReadSocks5Request();
      }
    }
  );
}

void socks5::Socks5Session::DoReadSocks5Request()
{
  auto self(shared_from_this());
  _inSocket.async_receive
  (
    ba::buffer(_socks5RequesåBuff, MAX_BUFFER_LENGTH),
    [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        socks5::Socks5Request socks5Request(_socks5RequesåBuff);
        //std::cout << socks5Request.ToString();
        
        DoResolve();
      }
    }
  );
}

void socks5::Socks5Session::DoResolve()
{
  socks5::Socks5Request socks5Request(_socks5RequesåBuff);
  auto self(shared_from_this());
  _resolver.async_resolve
  (
    socks5Request.GetDstAddr(),
    socks5Request.GetPort(),
    [this, self](boost::system::error_code error_code, ba::ip::tcp::resolver::iterator resolverIterator)
    {
      if (!error_code)
      {
        DoConnect(resolverIterator);
      }
      else
      {
        std::cerr << error_code.message() << std::endl;
      }
    }
  );
}

void socks5::Socks5Session::DoConnect(ba::ip::tcp::resolver::iterator & resolverIterator)
{
  auto self(shared_from_this());
  _outSocket.async_connect
  (
    *resolverIterator,
    [this, self](boost::system::error_code error_code)
    {
      if (!error_code)
      {
        std::cout << "connect\n";

        uint32_t realRemoteIp = _outSocket.remote_endpoint().address().to_v4().to_ulong();
        uint16_t realRemotePort = htons(_outSocket.remote_endpoint().port());

        std::cout << _outSocket.remote_endpoint().address().to_v4() << ':' << realRemotePort << '\n';

        socks5::Socks5Reply socks5Reply(realRemoteIp, realRemotePort);
        _socks5ReplyBuff = socks5Reply.GenerateReplyBuffer();

        DoWriteSocks5Reply();
      }
      else
      {
        std::cerr << error_code.message() << std::endl;
      }
    }
  );
}

void socks5::Socks5Session::DoWriteSocks5Reply()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_socks5ReplyBuff, _socks5ReplyBuff.size()),
    [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        DoRead(3);
      }
    }
  );
}

void socks5::Socks5Session::DoRead(int direction)
{
  auto self(shared_from_this());

  if (direction & 0x01)

  {
    _inSocket.async_receive
    (
      ba::buffer(_inBuffer, _inBuffer.size()),
      [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        /*for (int i = 0; i < _inBuffer.size(); ++i)
          std::cerr << (char)_inBuffer[i];*/
        DoWrite(1, length);
      }
      else
      {
        std::string gg = error_code.message();
        std::cerr << "dsfd" << error_code.message() << std::endl;
      }
    }
    );
  }

  if (direction & 0x02)
  {


    _outSocket.async_receive
    (
      ba::buffer(_outBuffer, _outBuffer.size()),
      [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        DoWrite(2, length);

      }
    }
    );
  }
}

void socks5::Socks5Session::DoWrite(int direction, std::size_t length)
{
  auto self(shared_from_this());

  switch (direction)
  {
  case 1:
    _outSocket.async_send
    (
      ba::buffer(_inBuffer, length),
      [this, self, direction](boost::system::error_code ec, std::size_t length)
      {
        if (!ec)
        {
          DoRead(direction);
        }
      }
    );
    break;
  case 2:
    _inSocket.async_send
    (
      ba::buffer(_outBuffer, length),
      [this, self, direction](boost::system::error_code ec, std::size_t length)
      {
        if (!ec)
        {
          DoRead(direction);
        }
      }
    );
    break;
  }
}
