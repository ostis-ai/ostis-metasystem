/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #include <iostream>
 #include <locale>
 #include <string>


#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "keynodes/language_recognition_keynodes.hpp"
#include <string>
#include <utility>

#include "language-recognition-agent.hpp"

using namespace languageRecognitionModule;


ScResult LanguageRecognitionAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [messageAddr] = action.GetArguments<1>();

  SC_LOG_DEBUG("LanguageRecognitionAgent started");

  ScAddrVector messageLinks;
  ScAddr const translationNode = utils::IteratorUtils::getAnyByInRelation(
    &m_context, messageAddr, LanguageRecognitionKeynodes::nrel_sc_text_translation);

  ScIterator3Ptr const linkIterator = m_context.CreateIterator3(translationNode, ScType::ConstPermPosArc, ScType::NodeLink);
  ScAddr messageLink;
  while (linkIterator->Next())
    messageLink = linkIterator->Get(2);

  std::string messageText;
  m_context.GetLinkContent(messageLink, messageText);

  ScAddr languageClass = recognizeLanguage(messageText);

  m_context.GenerateConnector(ScType::ConstPermPosArc, languageClass, messageAddr);

  return action.FinishSuccessfully();
}

ScAddr LanguageRecognitionAgent::recognizeLanguage(std::string message)
{
  int enCount = 0, ruCount = 0;
  for (int i = 0; message[i]; ++i) 
  { 
    if (std::isalpha(message[i], std::locale("en_US.utf8")))
      enCount++;
    if (std::isalpha(message[i], std::locale("ru_RU.utf8")))
      ruCount++;
  }
  if (enCount > ruCount)
    return LanguageRecognitionKeynodes::lang_en;
  else return ScKeynodes::lang_ru;
}

ScAddr LanguageRecognitionAgent::GetActionClass() const
{
  return LanguageRecognitionKeynodes::action_language_recognition;
}
