/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/translation_keynodes.hpp"

#include "structure-translation-agent.hpp"

#include "translator/StructureTranslatorSet.hpp"

#include <string>
#include <sstream>

namespace structureTranslationModule
{


ScResult StructureTranslationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [structuresSet, answerAddr, lang] = action.GetArguments<3>();

  SC_LOG_INFO("StructureTranslationAgent started");
  
  try
  {
    std::stringstream translation;

    ScIterator3Ptr const & structIterator = m_context.CreateIterator3(structuresSet, ScType::ConstPermPosArc, ScType::ConstNode);
    while (structIterator->Next())
    {
      ScAddr const & structAddr = structIterator->Get(2);
      translation << translateStructure(structAddr, lang, &m_context);
    }

    SC_LOG_DEBUG("StructureTranslationAgent: translation result is " << translation.str());

    ScAddr const & translationLink = m_context.CreateLink();
    if (m_context.IsElement(translationLink) == SC_FALSE)
      SC_THROW_EXCEPTION(utils::ScException, "StructureTranslationAgent: cannot create answer link");
    if (m_context.SetLinkContent(translationLink, translation.str()) == SC_FALSE)
      SC_THROW_EXCEPTION(utils::ScException, "StructureTranslationAgent: cannot set link content");

    m_context.GenerateConnector(ScType::ConstPermPosArc, answerAddr, translationLink);

    SC_LOG_INFO("StructureTranslationAgent finished");
    return action.FinishSuccessfully();
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    SC_LOG_INFO("StructureTranslationAgent finished with error");
    return action.FinishWithError();
  }
}

ScAddr StructureTranslationAgent::GetActionClass() const
{
  return TranslationKeynodes::action_translate_structures_into_natural_language;
}

std::string StructureTranslationAgent::translateStructure(ScAddr const & structAddr, ScAddr const & lang, ScMemoryContext * context)
{
  auto TranslatorSet = std::make_unique<StructureTranslatorSet>(context);
  auto translation = TranslatorSet->translate(structAddr, lang);
  return translation;
}
}  // namespace structureTranslationModule
