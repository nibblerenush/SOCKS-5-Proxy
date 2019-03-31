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
    [this, self](const boost::system::error_code & errorCode, std::size_t readedLength)
    {
      if (!errorCode)
      {
        try
        {
          Socks5RequestHandshake::Method neededMethod = static_cast<Socks5RequestHandshake::Method>(GetAuthenticationMethod());
          socks5::Socks5RequestHandshake socks5RequestHandshake(_socks5RequestHandshakeBuff, readedLength, neededMethod);
          std::cout << socks5RequestHandshake.ToString() << std::endl;

          socks5::Socks5ReplyHandshake socks5ReplyHandshake(socks5RequestHandshake);
          std::cout << socks5ReplyHandshake.ToString() << std::endl;

          _socks5ReplyHandshakeBuff = socks5ReplyHandshake.GenerateReplyHandshakeBuffer();
          WriteSocks5ReplyHandshake();
        }
        catch (socks5::Socks5Exception socks5Exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(ReadSocks5RequestHandshake)", socks5Exception.what()) << std::endl;
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
    [this, self](const boost::system::error_code & errorCode, std::size_t writedLength)
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

void socks5::Socks5SessionBase::ReadSocks5Request()
{
  auto self(shared_from_this());
  _inSocket.async_receive
  (
    ba::buffer(_socks5RequestBuff, _socks5RequestBuff.size()),
    [this, self](const boost::system::error_code & errorCode, std::size_t readedLength)
    {
      if (!errorCode)
      {
        try
        {
          socks5::Socks5Request socks5Request(_socks5RequestBuff, readedLength);
          //std::cout << socks5Request.ToString() << std::endl;
          Resolve(socks5Request.GetDstAddr(), socks5Request.GetDstPort());
        }
        catch (socks5::Socks5Exception socks5Exception)
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(Socks5Request)", socks5Exception.what()) << std::endl;
        }
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(Socks5Request)", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::Resolve(std::string dstAddr, std::string dstPort)
{
  auto self(shared_from_this());
  _resolver.async_resolve
  (
    dstAddr,
    dstPort,
    [this, self](const boost::system::error_code & errorCode, ba::ip::tcp::resolver::results_type resultsType)
    {
      if (!errorCode)
      {
        Connect(resultsType);
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_resolve", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::Connect(const ba::ip::tcp::resolver::results_type & resultsType)
{
  auto self(shared_from_this());
  _outSocket.async_connect
  (
    resultsType.begin()->endpoint(),
    [this, self](const boost::system::error_code & errorCode)
    {
      if (!errorCode)
      {
        uint32_t realRemoteIp = ::htonl(_outSocket.remote_endpoint().address().to_v4().to_ulong());
        uint16_t realRemotePort = ::htons(_outSocket.remote_endpoint().port());

        socks5::Socks5Reply socks5Reply(realRemoteIp, realRemotePort);
        //std::cout << socks5Reply.ToString() << std::endl;
        _socks5ReplyBuff = socks5Reply.GenerateReplyBuffer();

        WriteSocks5Reply();
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_connect", errorCode.message()) << std::endl;
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
    [this, self](const boost::system::error_code & errorCode, std::size_t writedLength)
    {
      if (!errorCode)
      {
        Read(BOTH);
      }
      else
      {
        std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(Socks5Reply)", errorCode.message()) << std::endl;
      }
    }
  );
}

void socks5::Socks5SessionBase::Read(Direction direction)
{
  auto self(shared_from_this());
  if (direction & RECV_FROM_IN_SEND_TO_OUT)
  {
    _inSocket.async_receive
    (
      ba::buffer(_inBuffer, _inBuffer.size()),
      [this, self](const boost::system::error_code & errorCode, std::size_t readedLength)
      {
        if (!errorCode)
        {
          Write(RECV_FROM_IN_SEND_TO_OUT, readedLength);
        }
        else
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(RECV_FROM_IN_SEND_TO_OUT::Read)", errorCode.message()) << std::endl;
          CloseSockets();
        }
      }
    );
  }
  
  if (direction & RECV_FROM_OUT_SEND_TO_IN)
  {
    _outSocket.async_receive
    (
      ba::buffer(_outBuffer, _outBuffer.size()),
      [this, self](const boost::system::error_code & errorCode, std::size_t readedLength)
      {
        if (!errorCode)
        {
          Write(RECV_FROM_OUT_SEND_TO_IN, readedLength);
        }
        else
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_receive(RECV_FROM_OUT_SEND_TO_IN::Read)", errorCode.message()) << std::endl;
          CloseSockets();
        }
      }
    );
  }
}

void socks5::Socks5SessionBase::Write(Direction direction, std::size_t writeLength)
{
  auto self(shared_from_this());
  switch (direction)
  {
  case RECV_FROM_IN_SEND_TO_OUT:
    _outSocket.async_send
    (
      ba::buffer(_inBuffer, writeLength),
      [this, self, direction](const boost::system::error_code & errorCode, std::size_t writedLength)
      {
        if (!errorCode)
        {
          Read(direction);
        }
        else
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(Write)", errorCode.message()) << std::endl;
          CloseSockets();
        }
      }
    );
    break;
  case RECV_FROM_OUT_SEND_TO_IN:
    _inSocket.async_send
    (
      ba::buffer(_outBuffer, writeLength),
      [this, self, direction](const boost::system::error_code & errorCode, std::size_t writedLength)
      {
        if (!errorCode)
        {
          Read(direction);
        }
        else
        {
          std::cerr << ErrorPrinter::GetErrorPrint(_sessionId, "async_send(Write)", errorCode.message()) << std::endl;
          CloseSockets();
        }
      }
    );
    break;
  }
}

void socks5::Socks5SessionBase::CloseSockets()
{
  _inSocket.close();
  _outSocket.close();
}
