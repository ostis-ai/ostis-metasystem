#pragma once

#include "sc-memory/kpm/sc_agent.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "FormSemanticNeighbourhoodTranslationAgent.generated.hpp"

namespace naturalLanguageProcessingModule
{
class FormSemanticNeighbourhoodTranslationAgent : public ScAgent
{
public:
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionNode);
};

}  // namespace naturalLanguageProcessingModule
