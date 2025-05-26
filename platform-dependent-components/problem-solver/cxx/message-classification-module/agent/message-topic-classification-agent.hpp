#pragma once

#include <sc-memory/sc_agent.hpp>

#include "manager/MessageTopicClassificationManager.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationAgent : public ScActionInitiatedAgent
{
  public:
   ScAddr GetActionClass() const override;
 
   ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::unique_ptr<MessageTopicClassificationManager> manager;

  bool checkActionClass(ScAddr const & actionAddr);

  void initFields();
};

}  // namespace messageClassificationModule
