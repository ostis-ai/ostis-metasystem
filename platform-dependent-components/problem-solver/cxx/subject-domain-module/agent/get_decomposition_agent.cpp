/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "constants/sc_search_aliases.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/subject_domain_keynodes.hpp"

#include "get_decomposition_agent.hpp"

using namespace utils;

namespace subjectDomainModule
{
SC_AGENT_IMPLEMENTATION(GetDecompositionAgent)
{
  ScAddr const questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_INFO("GetDecompositionAgent started");

  size_t level = 1;
  ScAddr const subjDomainAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr const levelAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr const langAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_3);
  ScAddr decompositionAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_4);

  if (!m_memoryCtx.IsElement(subjDomainAddr))
  {
    SC_LOG_ERROR("GetDecompositionAgent: subject domain node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (m_memoryCtx.IsElement(levelAddr))
    m_memoryCtx.GetLinkContent(levelAddr, level);
  if (!m_memoryCtx.IsElement(langAddr))
  {
    SC_LOG_ERROR("GetDecompositionAgent: language node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!m_memoryCtx.IsElement(decompositionAddr))
  {
    decompositionAddr = subjectDomainModule::subject_domain_keynodes::nrel_section_decomposition;
    SC_LOG_DEBUG(
        "GetDecompositionAgent: decomposition relation node not found. By default, "
        << m_memoryCtx.HelperGetSystemIdtf(decompositionAddr) << " is used.");
  }

  ScAddr answerLink = m_memoryCtx.CreateLink();

  ScAddrVector decomposition = GetDecomposition(subjDomainAddr, decompositionAddr);
  json answerJSON{
      {CommonUtils::getAddrHashString(subjDomainAddr),
       {{"idtf", CommonUtils::getMainIdtf(&m_memoryCtx, subjDomainAddr, {langAddr})},
        {"decomposition", GetJSONDecomposition(decomposition, level, langAddr, decompositionAddr)},
        {"position", 0}}}};

  std::string answerJSONContent = answerJSON.dump();
  SC_LOG_INFO("Result decomposition: " << answerJSONContent);
  m_memoryCtx.SetLinkContent(answerLink, answerJSONContent);

  ScAddrVector answerElements = {answerLink};

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, answerElements, true);
  SC_LOG_INFO("GetDecompositionAgent finished");
  return SC_RESULT_OK;
}

bool GetDecompositionAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
        subject_domain_keynodes::action_get_decomposition, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector GetDecompositionAgent::GetDecomposition(ScAddr const & subjDomainAddr, ScAddr const & decompositionAddr)
{
  SC_LOG_DEBUG("GetDecompositionAgent: main section is " << m_memoryCtx.HelperGetSystemIdtf(subjDomainAddr) << ".");
  ScAddrVector decomposition;

  ScTemplate decompositionTemplate;
  decompositionTemplate.Quintuple(
      ScType::NodeVar >> sc_search_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      subjDomainAddr,
      ScType::EdgeAccessVarPosPerm,
      decompositionAddr);
  decompositionTemplate.Quintuple(
      sc_search_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> sc_search_aliases::SECTION_NODE,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::rrel_1);
  ScTemplateSearchResult result;
  m_memoryCtx.HelperSearchTemplate(decompositionTemplate, result);
  if (!result.IsEmpty())
  {
    ScAddr tupleNode = result[0][sc_search_aliases::DECOMPOSITION_TUPLE];
    ScAddr subSection = result[0][sc_search_aliases::SECTION_NODE];
    SC_LOG_DEBUG("GetDecompositionAgent: subsection is " << m_memoryCtx.HelperGetSystemIdtf(subSection) << ".");
    decomposition.push_back(subSection);

    ScAddr nextSubSection = utils::IteratorUtils::getNextFromSet(&m_memoryCtx, tupleNode, subSection);
    while (m_memoryCtx.IsElement(nextSubSection))
    {
      decomposition.push_back(nextSubSection);
      subSection = nextSubSection;
      SC_LOG_DEBUG("GetDecompositionAgent: subsection is " << m_memoryCtx.HelperGetSystemIdtf(subSection) << ".");
      nextSubSection = utils::IteratorUtils::getNextFromSet(&m_memoryCtx, tupleNode, subSection);
    }
  }
  return decomposition;
}

json GetDecompositionAgent::GetJSONDecomposition(
    ScAddrVector const & decomposition,
    size_t level,
    ScAddr const & langAddr,
    ScAddr const & decompositionAddr)
{
  json decompositionJSON;
  for (size_t index = 0; index < decomposition.size(); ++index)
  {
    json item;
    item["position"] = index;
    item["idtf"] = CommonUtils::getMainIdtf(&m_memoryCtx, decomposition[index], {langAddr});
    if (level != 1)
      item["decomposition"] = GetJSONDecomposition(
          GetDecomposition(decomposition[index], decompositionAddr), level - 1, langAddr, decompositionAddr);
    else
      item["decomposition"] = {};
    decompositionJSON[CommonUtils::getAddrHashString(decomposition[index])] = item;
  }

  return decompositionJSON;
}
}  // namespace subjectDomainModule
