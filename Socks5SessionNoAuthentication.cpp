#include "Socks5SessionNoAuthentication.h"
#include "Socks5RequestHandshake.h"

socks5::Socks5SessionNoAuthentication::Socks5SessionNoAuthentication(ba::ip::tcp::socket && socket, uint16_t bufferSize, int sessionId):
  Socks5SessionBase(std::move(socket), bufferSize, sessionId)
{
}

void socks5::Socks5SessionNoAuthentication::Authenticate()
{
  // ��� ��� �������������� �� ���������, �� ������
  // �������� ������� ������ SOCKS5 �������
  ReadSocks5Request();
}

uint8_t socks5::Socks5SessionNoAuthentication::GetAuthenticationMethod() const
{
  return Socks5RequestHandshake::Method::NO_AUTHENTICATION_REQUIRED;
}
