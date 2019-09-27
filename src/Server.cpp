#include "Server.hpp"
#include "RequestHandshake.hpp"
#include "ReplyHandshake.hpp"
#include "Request.hpp"
#include "Reply.hpp"

#include "Exception.hpp"
#include <functional>


#include <iostream>
#include <memory>
#include <utility>

#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>



namespace socks5
{
  

  Server::Server(ba::io_context & io_context): coro1{}, coro2{}, coro3{}
  {
    ba::ip::tcp::endpoint serverEndpoint{ba::ip::tcp::v4(), 11080};
    _acceptor.reset(new ba::ip::tcp::acceptor{io_context, serverEndpoint});


  }

  #include <boost/asio/yield.hpp>

  void Server::operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results)
  {
    if (!ec)
    {
      reenter(coro2)
      {
        std::cout << results.begin()->endpoint().address().to_string() << std::endl;

        _fromRemoteSock.reset(new ba::ip::tcp::socket{_acceptor->get_io_context()});

        yield _fromRemoteSock->async_connect(results.begin()->endpoint(), *this);
      }
    }
  }

  void printBuff(const std::vector<uint8_t> & buf, std::size_t len)
  {
    for (std::size_t i {0}; i < len; ++i)
    {
      std::cout << buf[i];
    }
  }

  void Server::operator()(bs::error_code ec, std::size_t length)
  {
    if (!ec)
    {
      reenter(coro1)
      {
        do
        {
          //
          _fromLocalSock.reset(new ba::ip::tcp::socket{_acceptor->get_io_context()});

          //
          yield _acceptor->async_accept(*_fromLocalSock, *this);

          //if (coro1.is_parent()) std::cout << "is_parent1" << std::endl;

          //
          fork Server(*this)();

          //if (coro1.is_parent()) std::cout << "is_parent2" << std::endl;
          //if (coro1.is_child()) std::cout << "is_child" << std::endl;
        }
        while (coro1.is_parent());

        
        
        
        //
        _buffer.reset(new std::vector<uint8_t>);
        _buffer->resize(TEMP_BUFFER_SIZE);

        
        
        // ==================== Request/Reply Handshake ====================
        yield _fromLocalSock->async_receive(ba::buffer(*_buffer), *this);

        //yield _fromLocalSock->async_receive(ba::buffer(*_buffer), &Server::test);

        



        _requestHandshake.reset(new RequestHandshake{*_buffer, length, NO_AUTHENTICATION_REQUIRED});
        BOOST_LOG_TRIVIAL(info) << _requestHandshake->ToString();

        _replyHandshake.reset(new ReplyHandshake{NO_AUTHENTICATION_REQUIRED});
        yield _fromLocalSock->async_send(ba::buffer(_replyHandshake->GenBuff()), *this);
        BOOST_LOG_TRIVIAL(info) << _replyHandshake->ToString();
        // ==================== Request/Reply Handshake ====================

        // ==================== Request/Reply ====================
        yield _fromLocalSock->async_receive(ba::buffer(*_buffer), *this);
        _request.reset(new Request{*_buffer, length});
        BOOST_LOG_TRIVIAL(info) << _request->ToString();
        
        _resolver.reset(new ba::ip::tcp::resolver{_acceptor->get_io_context()});
        yield _resolver->async_resolve(_request->DstAddr(), _request->DstPort(), *this);

        //_reply.reset(new Reply{::htonl(_fromRemoteSock->remote_endpoint().address().to_v4().to_ulong()), _fromRemoteSock->remote_endpoint().port()});
        _reply.reset(new Reply{0, 0});
        yield _fromLocalSock->async_send(ba::buffer(_reply->GenBuff()), *this);
        BOOST_LOG_TRIVIAL(info) << _reply->ToString();
        // ==================== Request/Reply ====================

        _fromLocalBuff.reset(new std::vector<uint8_t>(BUFFER_SIZE));
        _fromRemoteBuff.reset(new std::vector<uint8_t>(BUFFER_SIZE));

        fork Server(*this)();

        if (coro1.is_parent())
        {
          do
          {
            yield _fromLocalSock->async_receive(ba::buffer(*_fromLocalBuff), *this);
            yield _fromRemoteSock->async_send(ba::buffer(*_fromLocalBuff, length), *this);
          }
          while (true);
        }

        do
        {
          yield _fromRemoteSock->async_receive(ba::buffer(*_fromRemoteBuff), *this);
          yield _fromLocalSock->async_send(ba::buffer(*_fromRemoteBuff, length), *this);
        }
        while (true);

      }
    }
    else
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ec.message() << std::endl;
    }
  }

  #include <boost/asio/unyield.hpp>
}
