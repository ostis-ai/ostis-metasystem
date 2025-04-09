/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>


namespace searchAnswerModule
{
class SearchPropertiesAgent : public ScActionInitiatedAgent
{
 public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  ScAddrVector findProperties(ScAddr const & conceptAddr);

  bool checkAction(ScAddr const & actionAddr);
};

}  // namespace SearchAnswerModule