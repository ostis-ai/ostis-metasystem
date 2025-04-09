#pragma once

#include <sc-memory/sc_agent.hpp>

namespace generateResponseModuleTest
{
class OneParameterTestAgent : public ScActionInitiatedAgent
{
  public:
   ScAddr GetActionClass() const override;
 
   ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  bool checkAction(ScAddr const & actionAddr);
};

}  // namespace generateResponseModuleTest
