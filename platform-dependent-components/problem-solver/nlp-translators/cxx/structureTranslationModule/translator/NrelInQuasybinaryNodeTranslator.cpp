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
  scTemplate.Triple(structAddr, ScType::VarPermPosArc, TranslationConstants::EDGE_ALIAS);
  scTemplate.Quintuple(
      ScType::VarNode >> TranslationConstants::NODE_ALIAS,
      TranslationConstants::EDGE_ALIAS,
      ScType::VarNodeTuple >> TranslationConstants::TUPLE_ALIAS,
      ScType::VarPermPosArc,
      ScType::VarNodeNonRole >> TranslationConstants::NREL_ALIAS);
  context->SearchByTemplateInterruptibly(
      scTemplate,
      [&](ScTemplateResultItem const & searchResult)
      {
        node = searchResult[TranslationConstants::NODE_ALIAS];
        if (isIgnored(node))
          return ScTemplateSearchRequest::CONTINUE;
        tupleNode = searchResult[TranslationConstants::TUPLE_ALIAS];
        nrelNode = searchResult[TranslationConstants::NREL_ALIAS];
        if (context->CheckConnector(
                TranslationKeynodes::translation_ignored_keynodes, nrelNode, ScType::ConstPermPosArc))
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
            context->CreateIterator3(tupleNode, ScType::ConstPermPosArc, ScType::ConstNode);

        int count = 0;
        while (tupleNodeIterator->Next())
        {
          ScAddr const & tupleElement = tupleNodeIterator->Get(2);
          std::string const & tupleElementMainIdtf =
              utils::CommonUtils::getMainIdtf(context, tupleElement, {lang});
          if (tupleElementMainIdtf.empty())
            continue;
          if(count == 0)
            translation << tupleElementMainIdtf;
          else 
          {
            if (count == TranslationConstants::MAX_LISTING_COUNT)
              translation << "|";
            translation << ", " << tupleElementMainIdtf;
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
  SC_LOG_DEBUG("NrelInQuasybinaryNodeTranslator " << translations.str());
  return translations;
}
}  // namespace structureTranslationModule
