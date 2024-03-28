/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "constants/subject_domain_aliases.hpp"
#include "keynodes/subject_domain_keynodes.hpp"

#include "subject_domain_generator.hpp"

namespace subjectDomainModule
{
ScAddr subject_domain_generator::GenerateSection(
    ScMemoryContext * context,
    std::string const & sectionName,
    ScAddr const & lang,
    bool isAtomic)
{
  ScAddr section = context->CreateNode(ScType::NodeConstClass);
  utils::CommonUtils::setMainIdtf(context, section, sectionName, {lang});

  ScTemplate sectionTemplate;
  sectionTemplate.Triple(subject_domain_keynodes::section, ScType::EdgeAccessVarPosPerm, section);
  if (isAtomic)
    sectionTemplate.Triple(subject_domain_keynodes::atomic_section, ScType::EdgeAccessVarPosPerm, section);
  else
    sectionTemplate.Triple(subject_domain_keynodes::non_atomic_section, ScType::EdgeAccessVarPosPerm, section);
  sectionTemplate.Triple(subject_domain_keynodes::not_enough_formed_structure, ScType::EdgeAccessVarPosPerm, section);
  ScTemplateGenResult genResult;
  context->HelperGenTemplate(sectionTemplate, genResult);

  return section;
}

}  // namespace subjectDomainModule
