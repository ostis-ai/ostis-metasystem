#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "ExampleKeynodes.generated.hpp"

namespace exampleModule
{
class ExampleKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_example"), ForceCreate)
  static ScAddr action_example;

};

}  // namespace exampleModule
