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
  SC_PROPERTY(Keynode("test_action"), ForceCreate)
  static ScAddr test_action;
};

}  // namespace generateResponseModuleTest
