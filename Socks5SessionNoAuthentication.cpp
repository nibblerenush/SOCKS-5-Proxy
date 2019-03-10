#include "Socks5SessionNoAuthentication.h"
#include "Socks5RequestHandshake.h"

void socks5::Socks5SessionNoAuthentication::Authenticate()
{
  // ��� ��� �������������� �� ���������,
  // �� ������ �������� ������� ������
  // SOCKS5 �������
  ReadSocks5Request();
}

uint8_t socks5::Socks5SessionNoAuthentication::GetAuthenticationMethod() const
{
  return Socks5RequestHandshake::Method::NO_AUTHENTICATION_REQUIRED;
}
