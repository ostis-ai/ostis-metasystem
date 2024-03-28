#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "OneParameterTestAgent.generated.hpp"

namespace generateResponseModuleTest
{
class OneParameterTestAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkAction(ScAddr const & actionAddr);
  
};

}  // namespace generateResponseModuleTest