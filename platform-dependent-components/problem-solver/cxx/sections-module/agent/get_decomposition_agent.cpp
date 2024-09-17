/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/sections_keynodes.hpp"

#include "get_decomposition_agent.hpp"
#include "constants/sections_aliases.hpp"

using namespace utils;

namespace sectionsModule
{
ScResult GetDecompositionAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr const questionNode = m_context.GetEdgeTarget(event.GetArc());
  if (!CheckActionClass(questionNode))
    return action.FinishSuccessfully();

  SC_LOG_INFO("GetDecompositionAgent started");

  size_t level = 1;

  std::tuple<ScAddr, ScAddr, ScAddr, ScAddr> const Tresult = action.GetArguments<4>();
  ScAddr const subjDomainAddr = std::get<0>(Tresult);
  ScAddr const levelAddr = std::get<1>(Tresult);
  ScAddr const langAddr = std::get<2>(Tresult);
  ScAddr decompositionAddr = std::get<3>(Tresult);

  if (!m_context.IsElement(subjDomainAddr))
  {
    SC_LOG_ERROR("GetDecompositionAgent: subject domain node not found.");
    return action.FinishUnsuccessfully();
  }
  if (m_context.IsElement(levelAddr))
    m_context.GetLinkContent(levelAddr, level);
  if (!m_context.IsElement(langAddr))
  {
    SC_LOG_ERROR("GetDecompositionAgent: language node not found.");
    return action.FinishUnsuccessfully();
  }
  if (!m_context.IsElement(decompositionAddr))
  {
    decompositionAddr = SectionsKeynodes::nrel_section_decomposition;
    SC_LOG_DEBUG(
        "GetDecompositionAgent: decomposition relation node not found. By default, "
        << m_context.HelperGetSystemIdtf(decompositionAddr) << " is used.");
  }

  ScAddr answerLink = m_context.CreateLink();

  ScAddrVector decomposition = GetDecomposition(subjDomainAddr, decompositionAddr);
  json answerJSON{
      {CommonUtils::getAddrHashString(subjDomainAddr),
       {{"idtf", CommonUtils::getMainIdtf(&m_context, subjDomainAddr, {langAddr})},
        {"decomposition", GetJSONDecomposition(decomposition, level, langAddr, decompositionAddr)},
        {"position", 0}}}};

  std::string answerJSONContent = answerJSON.dump();
  SC_LOG_INFO("Result decomposition: " << answerJSONContent);
  m_context.SetLinkContent(answerLink, answerJSONContent);

  ScAddrVector answerElements = {answerLink};

  ScStructure result = m_context.GenerateStructure();
  for (auto const & answerElement : answerElements)
    result << answerElement;
  action.SetResult(result);

  SC_LOG_INFO("GetDecompositionAgent finished");
  return action.FinishSuccessfully();
}

ScAddr GetDecompositionAgent::GetActionClass() const
{
  return SectionsKeynodes::action_get_decomposition;
}

bool GetDecompositionAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_context.HelperCheckEdge(
      SectionsKeynodes::action_get_decomposition, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector GetDecompositionAgent::GetDecomposition(ScAddr const & subjDomainAddr, ScAddr const & decompositionAddr)
{
  SC_LOG_DEBUG("GetDecompositionAgent: main section is " << m_context.HelperGetSystemIdtf(subjDomainAddr) << ".");
  ScAddrVector decomposition;

  ScTemplate decompositionTemplate;
  decompositionTemplate.Quintuple(
      subjDomainAddr,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      decompositionAddr);
  decompositionTemplate.Quintuple(
      sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> sections_aliases::SECTION_NODE,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);
  ScTemplateSearchResult result;
  m_context.HelperSearchTemplate(decompositionTemplate, result);
  if (!result.IsEmpty())
  {
    ScAddr tupleNode = result[0][sections_aliases::DECOMPOSITION_TUPLE];
    ScAddr subSection = result[0][sections_aliases::SECTION_NODE];
    SC_LOG_DEBUG("GetDecompositionAgent: subsection is " << m_context.HelperGetSystemIdtf(subSection) << ".");
    decomposition.push_back(subSection);

    ScAddr nextSubSection = utils::IteratorUtils::getNextFromSet(&m_context, tupleNode, subSection);
    while (m_context.IsElement(nextSubSection))
    {
      decomposition.push_back(nextSubSection);
      subSection = nextSubSection;
      SC_LOG_DEBUG("GetDecompositionAgent: subsection is " << m_context.HelperGetSystemIdtf(subSection) << ".");
      nextSubSection = utils::IteratorUtils::getNextFromSet(&m_context, tupleNode, subSection);
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
    item["idtf"] = CommonUtils::getMainIdtf(&m_context, decomposition[index], {langAddr});
    if (level != 1)
      item["decomposition"] = GetJSONDecomposition(
          GetDecomposition(decomposition[index], decompositionAddr), level - 1, langAddr, decompositionAddr);
    else
      item["decomposition"] = {};
    decompositionJSON[CommonUtils::getAddrHashString(decomposition[index])] = item;
  }

  return decompositionJSON;
}
}  // namespace sectionsModule
