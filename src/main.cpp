#include <csignal>
#include <cstdlib>
#include <iostream>

#include "Server.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
    return EXIT_FAILURE;
  }

  boost::asio::io_context ioContext;
  boost::asio::signal_set signals{ioContext};
  signals.add(SIGTERM);
  signals.async_wait
  (
    [&ioContext](boost::system::error_code ec, int signalNumber)
    {
      if (!ec)
      {
        std::cout << "Signal number: " << signalNumber << std::endl;
        ioContext.stop();
      }
      else
      {
        std::cerr << "Error code: " << ec.message() << std::endl;
      }
    }
  );
  
  socks5::Server{ioContext, argv[1]}();
  ioContext.run();
  return EXIT_SUCCESS;
}
