/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Ruslan Korshunov
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "constants/SubjDomainAliases.hpp"
#include "keynodes/Keynodes.hpp"

#include "SubjDomainGenerator.hpp"

namespace subjectDomainModule
{
ScAddr SubjDomainGenerator::generateSection(
      ScMemoryContext * context,
      std::string const & sectionName,
      ScAddr const & lang,
      bool isAtomic
)
{
  ScAddr section = context->CreateNode(ScType::NodeConstClass);
  utils::CommonUtils::setMainIdtf(context, section, sectionName, { lang });

  ScTemplate sectionTemplate;
  sectionTemplate.Triple(Keynodes::section, ScType::EdgeAccessVarPosPerm, section);
  if (isAtomic)
    sectionTemplate.Triple(Keynodes::atomic_section, ScType::EdgeAccessVarPosPerm, section);
  else
    sectionTemplate.Triple(Keynodes::non_atomic_section, ScType::EdgeAccessVarPosPerm, section);
  sectionTemplate.Triple(Keynodes::not_enough_formed_structure, ScType::EdgeAccessVarPosPerm, section);
  ScTemplateGenResult genResult;
  context->HelperGenTemplate(sectionTemplate, genResult);

  return section;
}

}
