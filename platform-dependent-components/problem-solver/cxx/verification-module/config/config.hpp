#include <mutex>
#include <map>
#include "sc-config/sc_config.hpp"

class Config
{
private:
  static std::mutex mutex;
  ScConfig scConfig;
  std::map<std::string, std::map<std::string, std::string>> configs;

  Config(std::string const & scConfig);

  ~Config()
  {
  }

public:
  Config(Config & other) = delete;

  void operator=(const Config &) = delete;

  static Config * getInstance();

  std::string getValue(std::string const & group, std::string const & field);
};
