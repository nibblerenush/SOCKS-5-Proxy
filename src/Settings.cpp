#include "Settings.hpp"
#include <boost/property_tree/json_parser.hpp>

namespace socks5
{
  static uint16_t DEFAULT_PORT = 1080;
  static uint8_t DEFAULT_AUTHENTICATION_METHOD = 0;

  Settings & Settings::Inst(const std::string & filename)
  {
    static Settings settings;    
    if (!filename.empty())
    {
      pt::read_json(filename, settings._tree);
    }
    return settings;
  }

  uint16_t Settings::Port() const
  {
    boost::optional<uint16_t> port = _tree.get_optional<uint16_t>("server.port");
    return port ? port.value() : DEFAULT_PORT;
  }

  uint8_t Settings::AuthMethod() const
  {
    boost::optional<uint8_t> authMethod = _tree.get_optional<uint8_t>("server.authentication_method");
    return authMethod ? authMethod.value() : DEFAULT_AUTHENTICATION_METHOD;
  }
}
