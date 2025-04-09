/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #pragma once

 #include <sc-memory/sc_agent.hpp>
 

namespace messageReplyModule
{
class MessageReplyAgent : public ScActionInitiatedAgent
{
  public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  int WAIT_TIME = 50000;

  bool checkActionClass(ScAddr const & actionAddr);

  static ScAddr getMessageProcessingProgram();

  ScAddr generateMessage(ScAddr const & linkAddr);

  ScAddr generateNonAtomicActionArgsSet(ScAddr const & messageAddr);

  ScAddr generateAnswer(ScAddr const & messageAddr);

  bool linkIsValid(ScAddr const & linkAddr);

  bool textLinkIsValid(ScAddr const & linkAddr);
};

}  // namespace messageReplyModule
