#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-memory/sc_addr.hpp"

#include "keynodes/message_classification_keynodes.hpp"

#include "message-topic-classification-agent.hpp"

using namespace messageClassificationModule;

ScResult MessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [messageAddr] = action.GetArguments<1>();

  SC_LOG_DEBUG("MessageTopicClassificationAgent started");

  initFields();
  ScAddrVector answerElements;

  try
  {
    SC_CHECK_PARAM(messageAddr, "MessageTopicClassificationAgent: invalid message node");
    answerElements = manager->manage({messageAddr});
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description());
    SC_LOG_DEBUG("MessageTopicClassificationAgent finished");\
    return action.FinishWithError();
  }

  SC_LOG_DEBUG("MessageTopicClassificationAgent finished");\
  return action.FinishSuccessfully();
}

void MessageTopicClassificationAgent::initFields()
{
  this->manager = std::make_unique<MessageTopicClassificationManager>(&m_context);
}

bool MessageTopicClassificationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_context.HelperCheckEdge(
      MessageClassificationKeynodes::action_message_topic_classification, actionAddr, ScType::EdgeAccessConstPosPerm);
}


ScAddr MessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_message_topic_classification;
}
