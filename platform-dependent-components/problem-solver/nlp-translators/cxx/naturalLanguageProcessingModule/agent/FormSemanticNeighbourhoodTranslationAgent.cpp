#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/TranslationKeynodes.hpp"

#include "manager/FormSemanticNeighbourhoodTranslationManager.hpp"

#include "FormSemanticNeighbourhoodTranslationAgent.hpp"

namespace naturalLanguageProcessingModule
{
SC_AGENT_IMPLEMENTATION(FormSemanticNeighbourhoodTranslationAgent)
{
  ScAddr const & actionNode = otherAddr;
  try
  {
    if (checkActionClass(actionNode) == SC_FALSE)
      return SC_RESULT_OK;
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent started");

    ScAddr const & nodeAddr =
        utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);
    if (nodeAddr.IsValid() == SC_FALSE)
      SC_THROW_EXCEPTION(
          utils::ExceptionItemNotFound, "FormSemanticNeighbourhoodTranslationAgent: nodeAddr is not valid");

    auto manager = std::make_unique<FormSemanticNeighbourhoodTranslationManager>(&m_memoryCtx);
    ScAddrVector const & answerElements = manager->manage({nodeAddr});

    if (answerElements.empty())
      SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationAgent: answer is empty");

    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, answerElements, true);
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent finished");
    return SC_RESULT_OK;
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent finished with error");
    return SC_RESULT_ERROR;
  }
}

bool FormSemanticNeighbourhoodTranslationAgent::checkActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      TranslationKeynodes::action_form_semantic_neighbourhood_translation, actionNode, ScType::EdgeAccessConstPosPerm);
}

}  // namespace naturalLanguageProcessingModule
