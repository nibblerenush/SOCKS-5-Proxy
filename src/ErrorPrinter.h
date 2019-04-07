#ifndef _ERROR_PRINTER_H_
#define _ERROR_PRINTER_H_

#include <string>

namespace socks5
{
  class ErrorPrinter
  {
  public:
    static std::string GetErrorPrint(
      int sessionId,
      const std::string & operation,
      const std::string & message);
  };
}

#endif
