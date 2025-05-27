/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>


namespace generateResponseModule
{
class GenerateResponseAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  using ScAddrSet = std::set<ScAddr, ScAddrLessFunc>;

  bool checkAction(ScAddr const & actionAddr);

  ScAction createActionNode(ScAddr const & message);

  ScAddr FindResponseActionClass(ScAddr const & message);

  void processParamsFromMessage(
      ScAddr const & message,
      ScAddr const & action,
      ScAddr const & actionNode,
      ScAddrSet & mappedRelations);

  void attachAnswer(ScAddr const & messageAnswer, ScAddr const & messageAddr, ScAddr const & answerAddr);

  static void validateAddrWithInvalidParamException(ScAddr const & addr, std::string const & paramName);
};

}  // namespace generateResponseModule
