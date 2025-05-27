#pragma once

#include <string>

namespace messageClassificationModule
{
class WitAiConstants
{
public:
  static std::string const intents;
  static std::string const traits;
  static std::string const entities;
  static std::string const name;
  static std::string const value;
  static std::string const body;
};

class MessageClassificationAliasConstants
{
public:
  static std::string const setOfTraitsClassAlias;
  static std::string const traitIncludedClassLinkAlias;
  static std::string const setOfTraitsClassLinkAlias;
  static std::string const entityRoleAlias;
  static std::string const entitySetAlias;
  static std::string const entityRoleLinkAlias;
  static std::string const entityClassLinkAlias;
};

}  // namespace messageClassificationModule
