/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-memory/sc_addr.hpp"

#include "keynodes/search_answer_keynodes.hpp"
#include "constants/search_answer_constants.hpp"
#include <string>

#include "search-properties-agent.hpp"

using namespace searchAnswerModule;


ScResult SearchPropertiesAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [conceptAddr, langAddr] = action.GetArguments<2>();

  SC_LOG_DEBUG("SearchPropertiesAgent started");

  ScStructure answer = m_context.GenerateStructure();

  ScTemplate answerTemplate;
  answerTemplate.Quintuple(
        conceptAddr,
        ScType::VarCommonArc >> SearchAnswerConstants::COMMON_EDGE,
        ScType::VarNode >> SearchAnswerConstants::SET_NODE,
        ScType::VarPermPosArc >> SearchAnswerConstants::RREL_EDGE,
        SearchAnswerKeynodes::nrel_properties);

  ScAddrVector propertiesVector = findProperties(conceptAddr, langAddr);
  
  size_t i = 0;
  while (i < propertiesVector.size())
  {
    // ScAddr const propertyLink = m_context->GenerateLink();
    // m_context->SetLinkContent(propertyLink, propertiesVector[i]);
    std::string EDGE_ALIAS = SearchAnswerConstants::VAR_EDGE + std::to_string(i);
    answerTemplate.Triple(SearchAnswerConstants::SET_NODE, ScType::VarPermPosArc >> EDGE_ALIAS, propertiesVector[i]);
    i++;
  }

  ScTemplateGenResult templateGenResult;
  m_context.GenerateByTemplate(answerTemplate, templateGenResult);
  answer << templateGenResult;

  action.SetResult(answer);

  return action.FinishSuccessfully();
}

ScAddrVector SearchPropertiesAgent::findProperties(ScAddr const & conceptAddr, ScAddr const & langAddr)
{
  static const ScAddrToValueUnorderedMap<std::string> none =
{
    {ScKeynodes::lang_ru, "нет"},
    {SearchAnswerKeynodes::lang_en, "none"}
};
  ScAddrVector propertiesVector;
  std::string propertyName;
  
  auto const & propertiesIterator = m_context.CreateIterator3(ScType::ConstNodeClass, ScType::ConstPermPosArc, conceptAddr);
  while (propertiesIterator->Next())
  {
    ScAddr const & propertyNode = propertiesIterator->Get(0);
    if (m_context.CheckConnector(SearchAnswerKeynodes::concept_property, propertyNode, ScType::ConstPermPosArc))
    {
      propertyName = utils::CommonUtils::getMainIdtf(&m_context, propertyNode, {langAddr});
      // propertyName = strchr(propertyName, " ");
      propertyName = propertyName.substr(0, propertyName.find(" "));

      ScAddr const propertyLink = m_context.GenerateLink();
      m_context.SetLinkContent(propertyLink, propertyName);
      propertiesVector.push_back(propertyLink);
    }
  }

  if (propertiesVector.empty())
  {
    ScAddr const propertyLink = m_context.GenerateLink();
    auto const & noneIterator = none.find(langAddr);
    m_context.SetLinkContent(propertyLink, ((noneIterator != none.cend()) ? noneIterator->second : "none"));
    propertiesVector.push_back(propertyLink);
  }

  return propertiesVector;
}

// std::vector<std::string> SearchPropertiesAgent::findProperties(ScAddr const & conceptAddr)
// {
//   std::vector<std::string> propertiesVector;
//   std::string propertyName;
  
//   auto const & propertiesIterator = m_context.CreateIterator3(ScType::ConstNodeClass, ScType::ConstPermPosArc, conceptAddr);
//   while (propertiesIterator->Next())
//   {
//     ScAddr const & propertyNode = variablesIterator->Get(2);
//     if (m_context.CheckConnector(SearchAnswerKeynodes::concept_property, propertyNode, ScType::ConstPermPosArc))
//     {
//       propertyName = utils::CommonUtils::getMainIdtf(&m_context, propertyNode, {langAddr});
//       propertyName = strchr(propertyName, " ");
//       propertiesVector.push_back(propertyName);
//     }
//     if (propertiesVector.empty())
//       propertiesVector.push_back("none");
//   }

//   return propertiesVector;
// }


bool SearchPropertiesAgent::checkAction(ScAddr const & actionAddr)
{
  return m_context.CheckConnector(SearchAnswerKeynodes::action_search_properties, actionAddr, ScType::ConstPermPosArc);
}


ScAddr SearchPropertiesAgent::GetActionClass() const
{
  return SearchAnswerKeynodes::action_search_properties;
}
