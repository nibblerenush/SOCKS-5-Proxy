#include "Helper.h"
#include <sstream>

boost::property_tree::ptree socks5::Helper::Tree;

const uint16_t socks5::Helper::DEFAULT_SERVER_PORT = 1080;
const uint16_t socks5::Helper::DEFAULT_BUFFER_SIZE = 1024;

std::string socks5::Helper::GetPrintUsage()
{
  std::ostringstream result;
  result << "Usage: SOCKS-5-Proxy " << "[configuration file]";
  return result.str();
}

void socks5::Helper::SetFilename(const char * filename)
{
  boost::property_tree::read_ini(filename, Tree);
}

uint16_t socks5::Helper::GetServerPort()
{ 
  return Tree.get<uint16_t>("server.port", DEFAULT_SERVER_PORT);
}

uint16_t socks5::Helper::GetBufferSize()
{
  return Tree.get<uint16_t>("buffer.size", DEFAULT_BUFFER_SIZE);
}
