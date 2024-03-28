#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "OneParameterTestAgent.hpp"

using namespace generateResponseModuleTest;

SC_AGENT_IMPLEMENTATION(OneParameterTestAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr)) {
      return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("OneParameterTestAgent started");

  ScAddr parameterAddr = utils::IteratorUtils::getAnyByOutRelation(
    &m_memoryCtx, 
    otherAddr, 
    scAgentsCommon::CoreKeynodes::rrel_1);

  if (!parameterAddr.IsValid())
  {
    SC_LOG_ERROR("OneParameterTestAgent: missing parameter node");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  SC_LOG_DEBUG("OneParameterTestAgent finished");

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, {parameterAddr});
  return SC_RESULT_OK;
}

bool OneParameterTestAgent::checkAction(ScAddr const & actionAddr) 
{
    return m_memoryCtx.HelperCheckEdge(
            TestKeynodes::test_action,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}
