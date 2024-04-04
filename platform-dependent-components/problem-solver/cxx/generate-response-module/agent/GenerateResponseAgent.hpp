/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "GenerateResponseAgent.generated.hpp"

namespace generateResponseModule
{
class GenerateResponseAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()
private:
  class MessageParametersTemplate
  {
  public:
    MessageParametersTemplate();
    operator ScTemplate const &() const;

  private:
    ScTemplate templ;
  };

  bool checkAction(ScAddr const & actionAddr);

  ScAddr getMessageClassAddr(ScAddr const & messageAddr);

  ScAddr getMessageActionAddr(ScAddr const & messageAddr);

  ScAddrVector getMessageParameters(ScAddr const & messageAddr);

  bool attachAnswer(ScAddr const & messageAnswer, ScAddr const & messageAddr, ScAddr const & answerAddr);

  void validateAddrWithInvalidParamException(ScAddr const & addr, std::string const & paramName);

  void validateAddrWithItemNotFoundException(ScAddr const & addr, std::string const & message);

  void finishWorkWithMessage(std::string const & message, ScAddr const & questionNode, bool isSuccess);
};

}  // namespace generateResponseModule
