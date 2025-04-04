/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "get_decomposition_agent.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/sections_keynodes.hpp"
#include "constants/sections_aliases.hpp"

using namespace utils;

namespace sectionsModule
{
GetDecompositionAgent::GetDecompositionAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel::Debug);
}

ScResult GetDecompositionAgent::DoProgram(ScAction & action)
{
  size_t level = 1;

  auto [subjDomainAddr, levelAddr, langAddr, decompositionAddr] = action.GetArguments<4>();

  if (!m_context.IsElement(subjDomainAddr))
  {
    m_logger.Error("Subject domain node not found.");
    return action.FinishUnsuccessfully();
  }
  if (m_context.IsElement(levelAddr))
    m_context.GetLinkContent(levelAddr, level);
  if (!m_context.IsElement(langAddr))
  {
    m_logger.Error("Language node not found.");
    return action.FinishUnsuccessfully();
  }
  if (!m_context.IsElement(decompositionAddr))
  {
    decompositionAddr = SectionsKeynodes::nrel_section_decomposition;
    m_logger.Debug(
        "Decomposition relation node not found. By default, ",
        m_context.GetElementSystemIdentifier(decompositionAddr),
        " is used.");
  }

  ScAddr answerLink = m_context.GenerateLink();

  ScAddrVector decomposition = GetDecomposition(subjDomainAddr, decompositionAddr);
  json answerJSON{
      {CommonUtils::getAddrHashString(subjDomainAddr),
       {{"idtf", CommonUtils::getMainIdtf(&m_context, subjDomainAddr, {langAddr})},
        {"decomposition", GetJSONDecomposition(decomposition, level, langAddr, decompositionAddr)},
        {"position", 0}}}};

  std::string answerJSONContent = answerJSON.dump();
  m_logger.Info("Result decomposition: ", answerJSONContent);
  m_context.SetLinkContent(answerLink, answerJSONContent);

  ScStructure result = m_context.GenerateStructure();
  result << answerLink;
  action.SetResult(result);

  return action.FinishSuccessfully();
}

ScAddr GetDecompositionAgent::GetActionClass() const
{
  return SectionsKeynodes::action_get_decomposition;
}

ScAddrVector GetDecompositionAgent::GetDecomposition(ScAddr const & subjDomainAddr, ScAddr const & decompositionAddr)
{
  m_logger.Debug("Main section is ", m_context.GetElementSystemIdentifier(subjDomainAddr), ".");
  ScAddrVector decomposition;

  ScTemplate decompositionTemplate;
  decompositionTemplate.Quintuple(
      subjDomainAddr,
      ScType::VarCommonArc,
      ScType::VarNode >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc,
      decompositionAddr);
  decompositionTemplate.Quintuple(
      sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc,
      ScType::VarNode >> sections_aliases::SECTION_NODE,
      ScType::VarPermPosArc,
      ScKeynodes::rrel_1);
  ScTemplateSearchResult result;
  m_context.SearchByTemplate(decompositionTemplate, result);
  if (!result.IsEmpty())
  {
    ScAddr tupleNode = result[0][sections_aliases::DECOMPOSITION_TUPLE];
    ScAddr subSection = result[0][sections_aliases::SECTION_NODE];
    m_logger.Debug("Subsection is ", m_context.GetElementSystemIdentifier(subSection), ".");
    decomposition.push_back(subSection);

    ScAddr nextSubSection = utils::IteratorUtils::getNextFromSet(&m_context, tupleNode, subSection);
    while (m_context.IsElement(nextSubSection))
    {
      decomposition.push_back(nextSubSection);
      subSection = nextSubSection;
      m_logger.Debug("Subsection is ", m_context.GetElementSystemIdentifier(subSection), ".");
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
    std::string identifier = CommonUtils::getMainIdtf(&m_context, decomposition[index], {langAddr});
    if (identifier.empty())
    {
      identifier = m_context.GetElementSystemIdentifier(decomposition[index]);
      if (identifier.empty())
        identifier = "...";
    }
    item["idtf"] = identifier;
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
