#include "Helper.h"
#include "Socks5Server.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << socks5::Helper::GetPrintUsage();
    return EXIT_FAILURE;
  }
  else
  {
    socks5::Helper::SetFilename(argv[1]);
  }

  try
  {
    boost::asio::io_context io_context;
    socks5::Socks5Server socks5Server(io_context, socks5::Helper::GetServerPort(), socks5::Helper::GetBufferSize());
    io_context.run();
  }
  catch (std::exception & exception)
  {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
