#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "TestKeynodes.generated.hpp"

namespace generateResponseModuleTest
{
class TestKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_one_param"), ForceCreate)
  static ScAddr action_one_param;

  SC_PROPERTY(Keynode("action_zero_param"), ForceCreate)
  static ScAddr action_zero_param;
};

}  // namespace generateResponseModuleTest
