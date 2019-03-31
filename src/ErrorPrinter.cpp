#include "ErrorPrinter.h"

#include <ctime>
#include <iomanip>
#include <sstream>

std::string socks5::ErrorPrinter::GetErrorPrint(int sessionId, const std::string & operation, const std::string & message)
{
  std::time_t currentTime = std::time(nullptr);
  std::ostringstream result;
  result
    << std::put_time(std::localtime(&currentTime), "%H:%M:%S %d-%m-%Y")
    << " Session Id: " << sessionId
    << " Operaion: " << operation
    << " Error message: " << message;
  return result.str();
}
