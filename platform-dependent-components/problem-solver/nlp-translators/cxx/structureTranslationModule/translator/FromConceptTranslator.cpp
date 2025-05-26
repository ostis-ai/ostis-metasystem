/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "constants/translation_constants.hpp"
#include "keynodes/translation_keynodes.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-memory/sc_keynodes.hpp"

#include "FromConceptTranslator.hpp"

namespace structureTranslationModule
{
FromConceptTranslator::FromConceptTranslator(ScMemoryContext * context)
  : StructureTranslator(context)
{
}

std::stringstream FromConceptTranslator::translate(ScAddr const & structAddr, ScAddr const & lang) const
{
  std::stringstream translations;
  ScAddr classNode;
  ScAddr node;

  ScTemplate scTemplate;
  scTemplate.Triple(
      ScType::NodeVarClass >> TranslationConstants::CLASS_ALIAS,
      ScType::EdgeAccessVarPosPerm >> TranslationConstants::EDGE_ALIAS,
      ScType::NodeVar >> TranslationConstants::NODE_ALIAS);
  scTemplate.Triple(structAddr, ScType::EdgeAccessVarPosPerm, TranslationConstants::EDGE_ALIAS);
  ScTemplateSearchResult searchResult;
  context->HelperSmartSearchTemplate(
      scTemplate,
      [&](ScTemplateResultItem const & searchResult)
      {
        classNode = searchResult[TranslationConstants::CLASS_ALIAS];
        if (isIgnored(classNode))
          return ScTemplateSearchRequest::CONTINUE;
        node = searchResult[TranslationConstants::NODE_ALIAS];
        std::string const & classMainIdtf =
            utils::CommonUtils::getMainIdtf(context, classNode, {lang});
        if (classMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & nodeMainIdtf =
            utils::CommonUtils::getMainIdtf(context, node, {lang});
        if (nodeMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        translations << nodeMainIdtf << " это " << classMainIdtf << ". ";
        SC_LOG_DEBUG("FromConceptTranslator" << translations.str());
        return ScTemplateSearchRequest::CONTINUE;
      },
      [&](ScAddr const & element)
      {
        return isInStructure(structAddr, element);
      });
  SC_LOG_DEBUG("FromConceptTranslator" << translations.str());
  return translations;
}

}  // namespace structureTranslationModule
