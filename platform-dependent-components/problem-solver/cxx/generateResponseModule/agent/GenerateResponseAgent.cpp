/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "keynodes/Keynodes.hpp"
#include "GenerateResponseAgent.hpp"

using namespace generateResponseModule;
using namespace utils;

SC_AGENT_IMPLEMENTATION(GenerateResponseAgent)
{
    ScAddr const & actionAddr = otherAddr;

    if (!checkAction(actionAddr)) {
        return SC_RESULT_OK;
    }

    SC_LOG_DEBUG("GenerateResponseAgent started");

    ScAddr messageAddr = IteratorUtils::getAnyFromSet(&m_memoryCtx, otherAddr);

    if (!messageAddr.IsValid()) 
    {
        SC_LOG_ERROR("GenerateResponseAgent: parameter 'messageAddr' is not valid");
        utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
        return SC_RESULT_ERROR;
    }
    
    ScAddr messageClassAddr = getMessageClassAddr(messageAddr);
    if (!messageClassAddr.IsValid())
    {
        SC_LOG_ERROR("GenerateResponseAgent: message class is not supported");
        utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
        return SC_RESULT_ERROR;
    }

    ScAddr messageActionAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, 
        messageClassAddr, 
        Keynodes::nrel_response_action);
    if (!messageActionAddr.IsValid())
    {
        SC_LOG_ERROR("GenerateResponseAgent: message class is not supported");
        utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
        return SC_RESULT_ERROR;
    }

    ScAddr messageAnswer = AgentUtils::applyActionAndGetResultIfExists(
        &m_memoryCtx, 
        messageActionAddr, 
        getMessageParameters(messageAddr));

    bool isSuccess = false;
    if (messageAnswer.IsValid())
    {
        isSuccess = true;
        ScAddr edgeBetweenMessageAndMessageAnswer = m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, messageAddr, messageAnswer);
        m_memoryCtx.CreateEdge(
            ScType::EdgeAccessConstPosPerm, 
            Keynodes::nrel_response, 
            edgeBetweenMessageAndMessageAnswer);
    }

    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, isSuccess);
    SC_LOG_DEBUG("GenerateResponseAgent finished");
    return SC_RESULT_OK;
}

bool GenerateResponseAgent::checkAction(ScAddr const & actionAddr) 
{
    return m_memoryCtx.HelperCheckEdge(
            Keynodes::action_generate_response,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}

ScAddr GenerateResponseAgent::getMessageClassAddr(ScAddr const & messageAddr)
{
    ScIterator3Ptr classIterator = m_memoryCtx.Iterator3(
        ScType::NodeConstClass, 
        ScType::EdgeAccessConstPosPerm, 
        messageAddr);

    while (classIterator->Next())
    {
        ScAddr messageClassAddr = classIterator->Get(0);
        bool isMessageClass = m_memoryCtx.HelperCheckEdge(
            Keynodes::concept_ask_ai_message, 
            messageClassAddr, 
            ScType::EdgeAccessConstPosPerm);
        if (isMessageClass)
            return messageClassAddr;
    }

    return {};
}

ScAddrVector GenerateResponseAgent::getMessageParameters(ScAddr const & messageAddr)
{
    //TODO: implement receiving several parameters from a message
    ScAddr messageParameter = IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, Keynodes::rrel_entity);

    ScAddrVector parameters;
    if (messageParameter.IsValid())
        parameters.emplace_back(messageParameter);

    return parameters;
}


