/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include "constants/translation_constants.hpp"

#include "NrelInQuasybinaryLinkTranslator.hpp"

namespace structureTranslationModule
{
NrelInQuasybinaryLinkTranslator::NrelInQuasybinaryLinkTranslator(ScMemoryContext * context)
  : StructureTranslator(context)
{
}

std::stringstream NrelInQuasybinaryLinkTranslator::translate(ScAddr const & structAddr, ScAddr const & lang) const
{
  std::stringstream translations;
  std::stringstream translation;
  ScAddr tupleNode;
  ScAddr node;
  ScAddr nrelNode;

  ScTemplate scTemplate;
  scTemplate.Triple(
      structAddr, ScType::EdgeAccessVarPosPerm, ScType::EdgeDCommonVar >> TranslationConstants::EDGE_ALIAS);
  scTemplate.Quintuple(
      ScType::NodeVar >> TranslationConstants::NODE_ALIAS,
      TranslationConstants::EDGE_ALIAS,
      ScType::NodeVarTuple >> TranslationConstants::TUPLE_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVarNoRole >> TranslationConstants::NREL_ALIAS);
  context->HelperSmartSearchTemplate(
      scTemplate,
      [&](ScTemplateResultItem const & searchResult)
      {
        node = searchResult[TranslationConstants::NODE_ALIAS];
        tupleNode = searchResult[TranslationConstants::TUPLE_ALIAS];
        nrelNode = searchResult[TranslationConstants::NREL_ALIAS];

        std::string const & nodeMainIdtf =
            utils::CommonUtils::getMainIdtf(context, node, {lang});
        if (nodeMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & nrelMainIdtf =
            utils::CommonUtils::getMainIdtf(context, nrelNode, {lang});
        if (nrelMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;

        auto const & linkIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);

        int count = 0;
        while (linkIterator->Next())
        {
          ScAddr const & linkNode = linkIterator->Get(2);
          if (!context->CheckConnector(lang, linkNode, ScType::ConstPermPosArc))
            continue;
          std::string linkContent;
          context->GetLinkContent(linkNode, linkContent);
          if (linkContent.empty())
            continue;

          if(count == 0)
            translation << linkContent;
          else 
          {
            if (count == TranslationConstants::MAX_LISTING_COUNT)
              translation << "|";
            translation << ", " << linkContent;
          }
          count++;
        }
        if (!(translation.tellp() == 0))
          translations << nodeMainIdtf << " " << nrelMainIdtf << " " << translation.str() << " ";
        translation.str("");
        return ScTemplateSearchRequest::CONTINUE;
      },
      [&](ScAddr const & element)
      {
        return isInStructure(structAddr, element);
      });
  SC_LOG_DEBUG("NrelInQuasybinaryLinkTranslator: " << translations.str());
  return translations;
}
}  // namespace structureTranslationModule
