#include <sc-memory/utils/sc_log.hpp>
#include <constants/verification_constants.hpp>
#include "config.hpp"

std::mutex Config::mutex;

Config::Config(std::string const & path)
  : scConfig(path, {verificationModule::FileConfigs::FILE_PATH})
{
}

Config * Config::getInstance()
{
  static Config instance(CONFIG_PATH);
  return &instance;
}

std::string Config::getValue(std::string const & groupName, std::string const & fieldName)
{
  std::lock_guard<std::mutex> lock(mutex);

  auto groupsIterator = configs.find(groupName);
  if (groupsIterator == configs.end())
    configs[groupName] = std::map<std::string, std::string>();

  std::map<std::string, std::string> & group = configs[groupName];
  auto groupFieldsIterator = group.find(fieldName);
  if (groupFieldsIterator == group.end())
  {
    ScConfigGroup scConfigGroup{scConfig[groupName]};
    std::string fieldValue = scConfigGroup[fieldName];
    group[fieldName] = fieldValue;
  }

  return configs[groupName][fieldName];
}
