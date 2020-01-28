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
  class RequestUnamePasswd;
  class ReplyUnamePasswd;
  class RequestLogin;
  class ReplyLogin;
  class RequestHmac;
  class ReplyHmac;

  class Server
  {
  public:
    explicit Server(ba::io_context & io_context, const std::string & filename);
    void operator()(bs::error_code ec = bs::error_code{}, std::size_t length = 0);
    void operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results);

  private:
    void CloseSockets();
    
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
    std::shared_ptr<std::vector<uint8_t>> _replyBuff{nullptr};

    std::shared_ptr<std::vector<uint8_t>> _serverHmac;

    std::shared_ptr<RequestHandshake> _requestHandshake{nullptr};
    std::shared_ptr<ReplyHandshake> _replyHandshake{nullptr};
    std::shared_ptr<RequestSocks> _requestSocks{nullptr};
    std::shared_ptr<ReplySocks> _replySocks{nullptr};
    std::shared_ptr<RequestUnamePasswd> _requestUnamePasswd{nullptr};
    std::shared_ptr<ReplyUnamePasswd> _replyUnamePasswd{nullptr};
    std::shared_ptr<RequestLogin> _requestLogin{nullptr};
    std::shared_ptr<ReplyLogin> _replyLogin{nullptr};
    std::shared_ptr<RequestHmac> _requestHmac{nullptr};
    std::shared_ptr<ReplyHmac> _replyHmac{nullptr};
  };
}
