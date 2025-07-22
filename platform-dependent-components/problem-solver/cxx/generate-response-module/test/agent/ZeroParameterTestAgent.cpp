#include "sc-memory/sc_addr.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "ZeroParameterTestAgent.hpp"

using namespace generateResponseModuleTest;

ScResult ZeroParameterTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  SC_LOG_DEBUG("ZeroParameterTestAgent started");

  ScAddr successNode = m_context.GenerateNode(ScType::ConstNode);
  m_context.SetElementSystemIdentifier("test_success", successNode);

  SC_LOG_DEBUG("ZeroParameterTestAgent ended");

  action.FormResult(successNode);
  return action.FinishSuccessfully();
}

ScAddr ZeroParameterTestAgent::GetActionClass() const
{
  return TestKeynodes::action_zero_param;
}

bool ZeroParameterTestAgent::checkAction(ScAddr const & action) 
{
    return m_context.CheckConnector(
            TestKeynodes::action_zero_param,
            action,
            ScType::ConstPermPosArc
            );
}
