#include "Server.hpp"
#include "RequestHandshake.hpp"
#include "ReplyHandshake.hpp"
#include "RequestSocks.hpp"
#include "ReplySocks.hpp"
#include "Exception.hpp"

#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>

#include <iostream>

namespace socks5
{
  Server::Server(ba::io_context & io_context):
  _majorCoro{},
  _minorCoro{},
  _serverEndpoint{ba::ip::tcp::v4(), 11080}
  {  
    _acceptor.reset(new ba::ip::tcp::acceptor{io_context, _serverEndpoint});
  }

  #include <boost/asio/yield.hpp>

  void Server::operator()(bs::error_code ec, std::size_t length)
  {
    if (!ec)
    {
      reenter(_majorCoro)
      {
        do
        {
          _localSock.reset(new ba::ip::tcp::socket{_acceptor->get_io_context()});
          
          yield _acceptor->async_accept(*_localSock, *this);

          fork Server(*this)();
        }
        while (_majorCoro.is_parent());
        
        _tempBuff.reset(new std::vector<uint8_t>(TEMP_BUFFER_SIZE));

        // ==================== Request/Reply Handshake ====================
        yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
        _requestHandshake.reset(new RequestHandshake{*_tempBuff, length, NO_AUTHENTICATION_REQUIRED});
        BOOST_LOG_TRIVIAL(info) << _requestHandshake->ToString();
        
        _replyHandshake.reset(new ReplyHandshake{NO_AUTHENTICATION_REQUIRED});
        yield _localSock->async_send(ba::buffer(_replyHandshake->GenBuff()), *this);
        BOOST_LOG_TRIVIAL(info) << _replyHandshake->ToString();
        // ==================== Request/Reply Handshake ====================

        // ==================== Request/Reply Socks ====================
        yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
        _requestSocks.reset(new RequestSocks{*_tempBuff, length});
        BOOST_LOG_TRIVIAL(info) << _requestSocks->ToString();
        
        _resolver.reset(new ba::ip::tcp::resolver{_acceptor->get_io_context()});
        yield _resolver->async_resolve(_requestSocks->DstAddr(), _requestSocks->DstPort(), *this);

        _replySocks.reset(new ReplySocks{_serverEndpoint.address().to_v4().to_uint(), _serverEndpoint.port()});
        yield _localSock->async_send(ba::buffer(_replySocks->GenBuff()), *this);
        BOOST_LOG_TRIVIAL(info) << _replySocks->ToString();
        // ==================== Request/Reply Socks ====================
        
        _localBuff.reset(new std::array<uint8_t, BUFFER_SIZE>());
        _remoteBuff.reset(new std::array<uint8_t, BUFFER_SIZE>());

        fork Server(*this)();

        if (_majorCoro.is_parent())
        {
          do
          {
            yield _localSock->async_receive(ba::buffer(*_localBuff), *this);
            yield _remoteSock->async_send(ba::buffer(*_localBuff, length), *this);
          }
          while (true);
        }

        do
        {
          yield _remoteSock->async_receive(ba::buffer(*_remoteBuff), *this);
          yield _localSock->async_send(ba::buffer(*_remoteBuff, length), *this);
        }
        while (true);
      }
    }
    else
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ec.message() << std::endl;
    }
  }
  
  void Server::operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results)
  {
    if (!ec)
    {
      reenter(_minorCoro)
      {
        //std::cout << results.begin()->endpoint().address().to_string() << std::endl;

        _remoteSock.reset(new ba::ip::tcp::socket{_acceptor->get_io_context()});
        
        yield _remoteSock->async_connect(results.begin()->endpoint(), *this);
      }
    }
    else
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ec.message() << std::endl;
    }
  }

  #include <boost/asio/unyield.hpp>
}
