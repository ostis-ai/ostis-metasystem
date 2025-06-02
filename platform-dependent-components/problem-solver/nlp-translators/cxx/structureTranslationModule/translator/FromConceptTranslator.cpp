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

  ScTemplate scTemplate;
  scTemplate.Triple(structAddr, ScType::VarPermPosArc, ScType::VarPermPosArc >> TranslationConstants::EDGE_ALIAS);
  context->SearchByTemplate(
      scTemplate,
      [&](ScTemplateResultItem const & searchResult)
      {
        ScAddr const & connector = searchResult[TranslationConstants::EDGE_ALIAS];
        auto const & [classNode, node] = context->GetConnectorIncidentElements(connector);
        if (!(context->GetElementType(classNode).BitAnd(ScType::NodeClass) == ScType::NodeClass) || !context->GetElementType(node).IsNode())
          return ScTemplateSearchRequest::CONTINUE;
        if (isIgnored(classNode))
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & classMainIdtf =
            utils::CommonUtils::getMainIdtf(context, classNode, {lang});
        if (classMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        std::string const & nodeMainIdtf =
            utils::CommonUtils::getMainIdtf(context, node, {lang});
        if (nodeMainIdtf.empty())
          return ScTemplateSearchRequest::CONTINUE;
        translations << nodeMainIdtf << " это " << classMainIdtf << ". ";
        SC_LOG_DEBUG("FromConceptTranslator " << translations.str());
        return ScTemplateSearchRequest::CONTINUE;
      },
      [&](ScAddr const & element)
      {
        return isInStructure(structAddr, element);
      });
  SC_LOG_DEBUG("FromConceptTranslator " << translations.str());
  return translations;
}

}  // namespace structureTranslationModule
