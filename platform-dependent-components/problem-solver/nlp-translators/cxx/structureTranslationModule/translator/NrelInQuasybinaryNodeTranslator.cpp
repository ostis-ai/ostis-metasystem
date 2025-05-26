/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include "keynodes/translation_keynodes.hpp"
#include "constants/translation_constants.hpp"

#include "NrelInQuasybinaryNodeTranslator.hpp"

namespace structureTranslationModule
{
NrelInQuasybinaryNodeTranslator::NrelInQuasybinaryNodeTranslator(ScMemoryContext * context)
  : StructureTranslator(context)
{
}

std::stringstream NrelInQuasybinaryNodeTranslator::translate(ScAddr const & structAddr, ScAddr const & lang) const
{
  std::stringstream translations;
  std::stringstream translation;
  ScAddr tupleNode;
  ScAddr node;
  ScAddr nrelNode;

  ScTemplate scTemplate;
  scTemplate.Triple(structAddr, ScType::EdgeAccessVarPosPerm, TranslationConstants::EDGE_ALIAS);
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
        if (isIgnored(node))
          return ScTemplateSearchRequest::CONTINUE;
        tupleNode = searchResult[TranslationConstants::TUPLE_ALIAS];
        nrelNode = searchResult[TranslationConstants::NREL_ALIAS];
        if (context->HelperCheckEdge(
                TranslationKeynodes::translation_ignored_keynodes, nrelNode, ScType::EdgeAccessConstPosPerm))
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & nodeMainIdtf =
            utils::CommonUtils::getMainIdtf(context, node, {lang});
        if (nodeMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & nrelMainIdtf =
            utils::CommonUtils::getMainIdtf(context, nrelNode, {lang});
        if (nrelMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;

        auto const & tupleNodeIterator =
            context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

        int count = 0;
        while (tupleNodeIterator->Next())
        {
          ScAddr const & tupleNode = tupleNodeIterator->Get(2);
          std::string const & tupleNodeMainIdtf =
              utils::CommonUtils::getMainIdtf(context, tupleNode, {lang});
          if (tupleNodeMainIdtf.empty())
            continue;
          if(count == 0)
            translation << tupleNodeMainIdtf;
          else 
          {
            if (count == TranslationConstants::MAX_LISTING_COUNT)
              translation << "|";
            translation << ", " << tupleNodeMainIdtf;
          }
          count++;
        }
        if (!(translation.tellp() == 0))
          translations << nodeMainIdtf << " " << nrelMainIdtf << " " << translation.str();
        translation.str("");
        return ScTemplateSearchRequest::CONTINUE;
      },
      [&](ScAddr const & element)
      {
        return isInStructure(structAddr, element);
      });
  SC_LOG_DEBUG("NrelInQuasybinaryNodeTranslator" << translations.str());
  return translations;
}
}  // namespace structureTranslationModule
