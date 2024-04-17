#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "LocalKeynodes.generated.hpp"

namespace commonModule
{
class LocalKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("lang_en"), ForceCreate)
  static ScAddr lang_en;
};

}  // namespace commonModule
