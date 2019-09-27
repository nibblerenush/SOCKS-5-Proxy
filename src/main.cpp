#include <csignal>
#include <cstdlib>
#include <iostream>

#include "Server.hpp"

int main(int argc, char ** argv)
{
  /*if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " [configuration file]";
    return EXIT_FAILURE;
  }

  try
  {
    boost::property_tree::ptree configurationFile;
    boost::property_tree::read_ini(argv[1], configurationFile);

    boost::asio::io_context ioContext;
    boost::asio::signal_set signals(ioContext);
    signals.add(SIGTERM);
    signals.async_wait
    (
      [&ioContext](const boost::system::error_code & errorCode, int signalNumber)
      {
        if (!errorCode)
        {
          std::cerr << "signalNumber: " << signalNumber << std::endl;
          ioContext.stop();
        }
        else
        {
          std::cerr << "errorCode: " << errorCode.message() << std::endl;
        }
      }
    );

    socks5::Socks5Server socks5Server(ioContext, configurationFile);
    ioContext.run();
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }*/

  try
  {
    boost::asio::io_context ioContext;
    socks5::Server{ioContext}();
    ioContext.run();
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  


  return EXIT_SUCCESS;



}
