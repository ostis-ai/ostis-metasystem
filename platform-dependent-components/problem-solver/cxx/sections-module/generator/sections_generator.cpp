/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "constants/sections_aliases.hpp"
#include "keynodes/sections_keynodes.hpp"

#include "sections_generator.hpp"

namespace sectionsModule
{
ScAddr sections_generator::GenerateSection(
    ScMemoryContext * context,
    std::string const & sectionName,
    ScAddr const & lang,
    bool isAtomic)
{
  ScAddr section = context->CreateNode(ScType::NodeConstClass);
  utils::CommonUtils::setMainIdtf(context, section, sectionName, {lang});

  ScTemplate sectionTemplate;
  sectionTemplate.Triple(SectionsKeynodes::section, ScType::EdgeAccessVarPosPerm, section);
  if (isAtomic)
    sectionTemplate.Triple(SectionsKeynodes::atomic_section, ScType::EdgeAccessVarPosPerm, section);
  else
    sectionTemplate.Triple(SectionsKeynodes::non_atomic_section, ScType::EdgeAccessVarPosPerm, section);
  sectionTemplate.Triple(SectionsKeynodes::not_enough_formed_structure, ScType::EdgeAccessVarPosPerm, section);
  ScTemplateGenResult genResult;
  context->HelperGenTemplate(sectionTemplate, genResult);

  return section;
}

}  // namespace subjectDomainModule
