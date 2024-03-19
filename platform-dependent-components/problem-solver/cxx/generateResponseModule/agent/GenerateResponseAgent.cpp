/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "keynodes/Keynodes.hpp"
#include "GenerateResponseAgent.hpp"

using namespace generateResponseModule;

SC_AGENT_IMPLEMENTATION(GenerateResponseAgent)
{
    ScAddr const & actionAddr = otherAddr;

    if (!checkAction(actionAddr)) {
        return SC_RESULT_OK;
    }

    SC_LOG_DEBUG("GenerateResponseAgent started");

    

    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
    SC_LOG_DEBUG("GenerateResponseAgent finished");
    return SC_RESULT_OK;
}

bool GenerateResponseAgent::checkAction(ScAddr const & actionAddr) {
    return m_memoryCtx.HelperCheckEdge(
            Keynodes::action_generate_response,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}

