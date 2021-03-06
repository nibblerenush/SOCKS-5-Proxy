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
  
  std::string Settings::Uname() const
  {
    return _tree.get<std::string>("auth.uname");
  }
  
  std::string Settings::Passwd() const
  {
    return _tree.get<std::string>("auth.passwd");
  }
  
  std::string Settings::Login() const
  {
    return _tree.get<std::string>("auth.login");
  }
  
  std::vector<uint8_t> Settings::HmacKey() const
  {
    std::string hmacKeyString = _tree.get<std::string>("auth.hmackey");
    std::vector<uint8_t> hmacKeyVector;
    std::copy(hmacKeyString.begin(),
              hmacKeyString.end(),
              std::back_inserter(hmacKeyVector));
    return hmacKeyVector;
  }
}
