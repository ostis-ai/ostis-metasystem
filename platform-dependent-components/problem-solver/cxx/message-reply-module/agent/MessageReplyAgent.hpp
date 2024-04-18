#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "MessageReplyAgent.generated.hpp"

namespace messageReplyModule
{
class MessageReplyAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  int WAIT_TIME = 50000;

  bool checkActionClass(ScAddr const & actionAddr);

  ScAddr getMessageProcessingProgram();

  ScAddr generateMessage(ScAddr const & linkAddr);

  ScAddr generateNonAtomicActionArgsSet(ScAddr const & messageAddr);

  ScAddr generateAnswer(ScAddr const & messageAddr);

  bool linkIsValid(ScAddr const & linkAddr);

  bool textLinkIsValid(ScAddr const & linkAddr);
};

}  // namespace messageReplyModule
