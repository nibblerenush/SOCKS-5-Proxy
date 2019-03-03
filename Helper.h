#ifndef _HELPER_H_
#define _HELPER_H_

#include <cstdint>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace socks5
{
  class Helper
  {
  public:
    static std::string GetPrintUsage();
    static void SetFilename(const char * filename);
    static uint16_t GetServerPort();
    static uint16_t GetBufferSize();

  private:
    static boost::property_tree::ptree Tree;
    static const uint16_t DEFAULT_SERVER_PORT;
    static const uint16_t DEFAULT_BUFFER_SIZE;
  };
}

#endif
