#include "Server.hpp"
#include "Settings.hpp"
#include "RequestHandshake.hpp"
#include "ReplyHandshake.hpp"
#include "RequestSocks.hpp"
#include "ReplySocks.hpp"
#include "RequestUnamePasswd.hpp"
#include "ReplyUnamePasswd.hpp"
#include "RequestLogin.hpp"
#include "ReplyLogin.hpp"
#include "RequestHmac.hpp"
#include "ReplyHmac.hpp"
#include "Exception.hpp"

#include <iostream>
#include <openssl-wrapper/Hmac.h>
#include <openssl-wrapper/BaseFunctions.h>
#include <openssl-wrapper/Initializer.h>

namespace socks5
{
  static const uint8_t UNAME_PASSWD_SUCCESS = 0;
  static const uint8_t UNAME_PASSWD_FAILURE = 1;

  Server::Server(ba::io_context & io_context, const std::string & filename):
  _majorCoro{},
  _minorCoro{},
  _serverEndpoint{ba::ip::tcp::v4(), Settings::Inst(filename).Port()}
  {  
    openssl_wrapper::Initialize();
    _acceptor.reset(new ba::ip::tcp::acceptor{io_context, _serverEndpoint});
  }

  #include <boost/asio/yield.hpp>

  void Server::operator()(bs::error_code ec, std::size_t length)
  {
    try
    {
      if (!ec)
      {
        reenter(_majorCoro)
        {
          do
          {
            _localSock.reset(new ba::ip::tcp::socket{_acceptor->get_executor()});
            
            yield _acceptor->async_accept(*_localSock, *this);

            fork Server(*this)();
          }
          while (_majorCoro.is_parent());
          
          _tempBuff.reset(new std::vector<uint8_t>(TEMP_BUFFER_SIZE));

          // ==================== Request/Reply Handshake ====================
          yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
          _requestHandshake.reset(new RequestHandshake{*_tempBuff, length, static_cast<Method>(Settings::Inst().AuthMethod())});
          
          _replyHandshake.reset(new ReplyHandshake{static_cast<Method>(Settings::Inst().AuthMethod())});
          _replyBuff.reset(new std::vector<uint8_t>{_replyHandshake->GenBuff()});
          yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
          // ==================== Request/Reply Handshake ====================

          // ==================== Authentication ====================
          if (static_cast<Method>(Settings::Inst().AuthMethod()) == USERNAME_PASSWORD)
          {
            yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
            _requestUnamePasswd.reset(new RequestUnamePasswd{*_tempBuff, length});
            
            if (_requestUnamePasswd->Uname() == Settings::Inst().Uname() && _requestUnamePasswd->Passwd() == Settings::Inst().Passwd())
            {
              _replyUnamePasswd.reset(new ReplyUnamePasswd{UNAME_PASSWD_SUCCESS});
              _replyBuff.reset(new std::vector<uint8_t>{_replyUnamePasswd->GenBuff()});
              yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
            }
            else
            {
              _replyUnamePasswd.reset(new ReplyUnamePasswd{UNAME_PASSWD_FAILURE});
              _replyBuff.reset(new std::vector<uint8_t>{_replyUnamePasswd->GenBuff()});
              yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
              _localSock->close();
              _localSock.reset();
              yield break;
            }
          }
          
          if (static_cast<Method>(Settings::Inst().AuthMethod()) == LOGIN_HMAC)
          {
            yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
            _requestLogin.reset(new RequestLogin{*_tempBuff, length});
            
            if (_requestLogin->Login() != Settings::Inst().Login())
            {
              _replyLogin.reset(new ReplyLogin{ReplyLogin::FAILURE});
              _replyBuff.reset(new std::vector<uint8_t>{_replyLogin->GenBuff()});
              yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
              _localSock->close();
              _localSock.reset();
              yield break;
            }
            
            _replyLogin.reset(new ReplyLogin{ReplyLogin::SUCCESS});
            _replyBuff.reset(new std::vector<uint8_t>{_replyLogin->GenBuff()});
            yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
            std::cout << _replyLogin->ToString() << std::endl;

            yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
            _requestHmac.reset(new RequestHmac(*_tempBuff, length));

            _serverHmac.reset(new std::vector<uint8_t>(openssl_wrapper::Hmac::GetMac("SHA256", _replyLogin->RandomString(), Settings::Inst().HmacKey())));


            using namespace openssl_wrapper;
            
            std::cout << BaseFunctions::GetHexString(_requestHmac->Hmac()) << ' ' << _requestHmac->Hmac().size() << std::endl;
            std::cout << BaseFunctions::GetHexString(*_serverHmac) << ' ' << _serverHmac->size() << std::endl;

            if (std::equal(_serverHmac->begin(), _serverHmac->end(), _requestHmac->Hmac().begin()))
            {
              _replyHmac.reset(new ReplyHmac{UNAME_PASSWD_SUCCESS});
              _replyBuff.reset(new std::vector<uint8_t>{_replyHmac->GenBuff()});
              yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
            }
            else
            {
              _replyHmac.reset(new ReplyHmac{UNAME_PASSWD_FAILURE});
              _replyBuff.reset(new std::vector<uint8_t>{_replyHmac->GenBuff()});
              yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
              _localSock->close();
              _localSock.reset();
              yield break;
            }

            
          }
          // ==================== Authentication ====================

          // ==================== Request/Reply Socks ====================
          yield _localSock->async_receive(ba::buffer(*_tempBuff), *this);
          _requestSocks.reset(new RequestSocks{*_tempBuff, length});
          
          _resolver.reset(new ba::ip::tcp::resolver{_acceptor->get_executor()});
          yield _resolver->async_resolve(_requestSocks->DstAddr(), _requestSocks->DstPort(), *this);

          _replySocks.reset(new ReplySocks{_serverEndpoint.address().to_v4().to_uint(), _serverEndpoint.port()});
          _replyBuff.reset(new std::vector<uint8_t>{_replySocks->GenBuff()});
          yield _localSock->async_send(ba::buffer(*_replyBuff), *this);
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
        CloseSockets();
      }
    }
    catch (socks5::Exception ex)
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ex.what() << std::endl;
      CloseSockets();
    }
    catch (openssl_wrapper::WrapperException ex)
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ex.what() << std::endl;
      CloseSockets();
    }
  }
  
  void Server::operator()(bs::error_code ec, ba::ip::tcp::resolver::results_type results)
  {
    if (!ec)
    {
      reenter(_minorCoro)
      {
        _remoteSock.reset(new ba::ip::tcp::socket{_acceptor->get_executor()});
        
        yield _remoteSock->async_connect(results.begin()->endpoint(), *this);
      }
    }
    else
    {
      std::cerr << __FILE__ << ": "<< __LINE__ << ": " << ec.message() << std::endl;
      CloseSockets();
    }
  }

  #include <boost/asio/unyield.hpp>
  
  void Server::CloseSockets()
  {
    if (_localSock)
    {
      _localSock->close();
      _localSock.reset();
    }
    
    if (_remoteSock)
    {
      _remoteSock->close();
      _remoteSock.reset();
    }
  }
}
