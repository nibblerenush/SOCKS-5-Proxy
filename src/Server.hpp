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
  class RequestSocks;
  class ReplySocks;

  class Server
  {
  public:
    explicit Server(ba::io_context & io_context);
    void operator()(bs::error_code ec = bs::error_code{}, std::size_t length = 0);
    void operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results);
    
  private:
    static const std::size_t BUFFER_SIZE{8192};
    static const std::size_t TEMP_BUFFER_SIZE{512};

  private:
    ba::coroutine _majorCoro;
    ba::coroutine _minorCoro;
    ba::ip::tcp::endpoint _serverEndpoint;

    std::shared_ptr<ba::ip::tcp::acceptor> _acceptor{nullptr};
    std::shared_ptr<ba::ip::tcp::resolver> _resolver{nullptr};
    std::shared_ptr<ba::ip::tcp::socket> _localSock{nullptr};
    std::shared_ptr<ba::ip::tcp::socket> _remoteSock{nullptr};
    std::shared_ptr<std::array<uint8_t, BUFFER_SIZE>> _localBuff{nullptr};
    std::shared_ptr<std::array<uint8_t, BUFFER_SIZE>> _remoteBuff{nullptr};
    std::shared_ptr<std::vector<uint8_t>> _tempBuff{nullptr};

    std::shared_ptr<RequestHandshake> _requestHandshake{nullptr};
    std::shared_ptr<ReplyHandshake> _replyHandshake{nullptr};
    std::shared_ptr<RequestSocks> _requestSocks{nullptr};
    std::shared_ptr<ReplySocks> _replySocks{nullptr};
  };
}
