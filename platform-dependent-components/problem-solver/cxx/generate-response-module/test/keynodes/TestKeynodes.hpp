#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace generateResponseModuleTest
{

class TestKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_one_param{
      "action_one_param",
      ScType::ConstNodeClass};
  static inline ScKeynode const action_zero_param{
      "action_zero_param",
      ScType::ConstNodeClass};
};

}  // namespace generateResponseModuleTest
