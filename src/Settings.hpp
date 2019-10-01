#pragma once

#include <cstdint>
#include <string>

#include <boost/property_tree/ptree.hpp>

namespace socks5
{
  namespace pt = boost::property_tree;
  
  class Settings
  {
  public:
    static Settings & Inst(const std::string & filename = std::string{});
    uint16_t Port() const;
    uint8_t AuthMethod() const;

  private:
    explicit Settings() = default;
    ~Settings() = default;
    Settings(const Settings &) = delete;
    Settings(const Settings &&) = delete;
    Settings & operator=(const Settings &) = delete;
    Settings & operator=(const Settings &&) = delete;
    
  private:
    pt::ptree _tree{};
  };
}
