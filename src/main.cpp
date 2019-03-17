#include "Socks5Server.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: SOCKS-5-Proxy " << "[configuration file]";
    return EXIT_FAILURE;
  }

  try
  {
    boost::property_tree::ptree configurationFile;
    boost::property_tree::read_ini(argv[1], configurationFile);

    boost::asio::io_context io_context;
    socks5::Socks5Server socks5Server(io_context, configurationFile);
    io_context.run();
  }
  catch (std::exception & exception)
  {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
