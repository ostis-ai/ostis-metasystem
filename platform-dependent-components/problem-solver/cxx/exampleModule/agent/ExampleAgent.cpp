#include "keynodes/ExampleKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "ExampleAgent.hpp"

using namespace exampleModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(ExampleAgent)
{
  ScAddr const & questionNode = otherAddr;
  if (!checkActionClass(questionNode))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("ExampleAgent started");

  // Write your agent logic here

  SC_LOG_DEBUG("ExampleAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, true);
  return SC_RESULT_OK;
}

bool ExampleAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
        ExampleKeynodes::action_example, actionAddr, ScType::EdgeAccessConstPosPerm);
}
