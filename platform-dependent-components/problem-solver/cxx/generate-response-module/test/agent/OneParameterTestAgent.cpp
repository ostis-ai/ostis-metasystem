// #include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "OneParameterTestAgent.hpp"

using namespace generateResponseModuleTest;

ScResult OneParameterTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  SC_LOG_DEBUG("OneParameterTestAgent started");

  ScAddr parameterAddr = utils::IteratorUtils::getAnyByOutRelation(
    &m_context, 
    action, 
    ScKeynodes::rrel_1);

  if (!parameterAddr.IsValid())
  {
    SC_LOG_ERROR("OneParameterTestAgent: missing parameter node");
    return action.FinishWithError();
  }

  SC_LOG_DEBUG("OneParameterTestAgent finished");

  return action.FinishSuccessfully();
}

bool OneParameterTestAgent::checkAction(ScAddr const & actionAddr) 
{
    return m_context.HelperCheckEdge(
            TestKeynodes::action_one_param,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}
