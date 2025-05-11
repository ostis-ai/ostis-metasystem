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

public:
  Config(Config & other) = delete;

  void operator=(Config const &) = delete;

  static Config * GetInstance();

  std::string GetValue(std::string const & groupName, std::string const & fieldName);
};
