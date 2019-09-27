#pragma once

#include <array>
#include <memory>
#include <boost/asio.hpp>

namespace socks5
{
  namespace ba = boost::asio;
  namespace bs = boost::system;

  class RequestHandshake;
  class ReplyHandshake;
  class Request;
  class Reply;

  class Server
  {
  public:
    Server(ba::io_context & io_context);
    void operator()(bs::error_code ec = bs::error_code{}, std::size_t length = 0);
    void operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results);
    
  private:
    static const std::size_t BUFFER_SIZE{8192};
    static const std::size_t TEMP_BUFFER_SIZE{512};

  private:
    ba::coroutine coro1;
    ba::coroutine coro2;

    std::shared_ptr<ba::ip::tcp::acceptor> _acceptor;
    std::shared_ptr<ba::ip::tcp::resolver> _resolver;
    std::shared_ptr<ba::ip::tcp::socket> _fromLocalSock;
    std::shared_ptr<ba::ip::tcp::socket> _fromRemoteSock;
    std::shared_ptr<std::vector<uint8_t>> _fromLocalBuff;
    std::shared_ptr<std::vector<uint8_t>> _fromRemoteBuff;
    std::shared_ptr<std::vector<uint8_t>> _buffer;

    std::shared_ptr<RequestHandshake> _requestHandshake;
    std::shared_ptr<ReplyHandshake> _replyHandshake;
    std::shared_ptr<Request> _request;
    std::shared_ptr<Reply> _reply;
  };
}
