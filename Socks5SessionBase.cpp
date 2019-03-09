#include "Socks5SessionBase.h"
#include "ErrorPrinter.h"
#include "Socks5Exception.h"
#include "Socks5Reply.h"
#include "Socks5ReplyHandshake.h"
#include "Socks5Request.h"
#include "Socks5RequestHandshake.h"

#include <iostream>

socks5::Socks5SessionBase::Socks5SessionBase(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId):
  _inSocket(std::move(socket)),
  _outSocket(_inSocket.get_io_context()),
  _socks5RequestHandshakeBuff(bufferSize),
  _socks5RequestBuff(bufferSize),
  _inBuffer(bufferSize),
  _outBuffer(bufferSize),
  _resolver(_inSocket.get_io_context()),
  _sessionId(sessionId)
{}

void socks5::Socks5SessionBase::Start()
{
  ReadSocks5RequestHandshake();
}

void socks5::Socks5SessionBase::ReadSocks5RequestHandshake()
{
  auto self(shared_from_this());
  _inSocket.async_receive
  (
    ba::buffer(_socks5RequestHandshakeBuff, _socks5RequestHandshakeBuff.size()),
    [this, self](boost::system::error_code errorCode, std::size_t readedLength)
    {
      if (!errorCode)
      {
        try
        {
          Socks5RequestHandshake::Method neededMethod = static_cast<Socks5RequestHandshake::Method>(GetAuthenticationMethod());
          socks5::Socks5RequestHandshake socks5RequestHandshake(_socks5RequestHandshakeBuff, readedLength, neededMethod);
          //std::cout << socks5RequestHandshake.ToString() << std::endl;
          
          socks5::Socks5ReplyHandshake socks5ReplyHandshake(socks5RequestHandshake);
          //std::cout << socks5ReplyHandshake.ToString();
          
          _socks5ReplyHandshakeBuff = socks5ReplyHandshake.GenerateReplyHandshakeBuffer();
          WriteSocks5ReplyHandshake();
        }
        catch (socks5::Socks5Exception socks5Exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5RequestHandshake)", socks5Exception.what()) << std::endl;
          return;
        }
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5RequestHandshake)", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::WriteSocks5ReplyHandshake()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_socks5ReplyHandshakeBuff, _socks5ReplyHandshakeBuff.size()),
    [this, self](boost::system::error_code errorCode, std::size_t writedLength)
    {
      if (!errorCode)
      {
        Authenticate();
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(WriteSocks5ReplyHandshake)", errorCode.message()) << std::endl;
      }
    }
  );
}

/*void socks5::Socks5SessionBase::ReadSocks5Request()
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
        std::cout << socks5Request.ToString();
        
        Resolve();
      }
    }
  );
}

void socks5::Socks5SessionBase::Resolve()
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
        Connect(resolverIterator);
      }
      else
      {
        std::cerr << error_code.message() << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::Connect(ba::ip::tcp::resolver::iterator & resolverIterator)
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

        WriteSocks5Reply();
      }
      else
      {
        std::cerr << error_code.message() << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::WriteSocks5Reply()
{
  auto self(shared_from_this());
  _inSocket.async_send
  (
    ba::buffer(_socks5ReplyBuff, _socks5ReplyBuff.size()),
    [this, self](boost::system::error_code error_code, std::size_t length)
    {
      if (!error_code)
      {
        Read(3);
      }
    }
  );
}

void socks5::Socks5SessionBase::Read(int direction)
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
        for (int i = 0; i < _inBuffer.size(); ++i)
          std::cerr << (char)_inBuffer[i];
        Write(1, length);
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
        Write(2, length);

      }
    }
    );
  }
}

void socks5::Socks5SessionBase::Write(int direction, std::size_t length)
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
          Read(direction);
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
          Read(direction);
        }
      }
    );
    break;
  }
}*/
