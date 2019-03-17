#include "ErrorPrinter.h"
#include <sstream>

std::string socks5::ErrorPrinter::GetErrorPrint(int sessionId, const std::string & operation, const std::string & message)
{
  std::ostringstream result;
  result << "Session Id: " << sessionId << " Operaion: " << operation << " Error message: " << message;
  return result.str();
}
