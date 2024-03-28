/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Ruslan Korshunov, Maksim Orlov
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "constants/SubjDomainAliases.hpp"
#include "keynodes/Keynodes.hpp"

#include "SubjDomainBuilder.hpp"

namespace subjectDomainModule
{
void SubjDomainBuilder::buildDecompositionTupleTemplate(
      ScTemplate & scTemplate,
      ScAddr const & parentSection,
      ScAddr const & childSection)
{
  scTemplate.Quintuple(
        ScType::NodeVar >> SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeDCommonVar,
        parentSection,
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_entity_decomposition
  );
  scTemplate.Triple(
        SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::EDGE_TO_DELETE,
        childSection
  );
}

void SubjDomainBuilder::buildDecompositionTupleTemplate(
      ScTemplate & scTemplate,
      ScAddr const & section)
{
  scTemplate.Quintuple(
        ScType::NodeVar >> SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeDCommonVar,
        ScType::NodeVar >> SubjDomainAliases::PARENT_SECTION,
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_entity_decomposition
  );
  scTemplate.Triple(
        SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::EDGE_TO_DELETE,
        section
  );
}

void SubjDomainBuilder::buildNextSectionTemplate(
      ScTemplate & nextSectionTemplate,
      ScAddr const & tuple,
      ScAddr const & section)
{
  nextSectionTemplate.Triple(
        tuple,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::SECTION_EDGE,
        section
  );
  nextSectionTemplate.Triple(
        tuple,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::NEXT_SECTION_ARC,
        ScType::NodeVar >> SubjDomainAliases::SECTION_NODE
  );
  nextSectionTemplate.Quintuple(
        SubjDomainAliases::SECTION_EDGE,
        ScType::EdgeDCommonVar >> SubjDomainAliases::BASE_SEQUENCE_EDGE,
        SubjDomainAliases::NEXT_SECTION_ARC,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_basic_sequence
  );
}

void SubjDomainBuilder::buildPreviousSectionTemplate(
      ScTemplate & previousSectionTemplate,
      ScAddr const & tuple,
      ScAddr const & section)
{
  previousSectionTemplate.Triple(
        tuple,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::SECTION_EDGE,
        section
  );
  previousSectionTemplate.Triple(
        tuple,
        ScType::EdgeAccessVarPosPerm >> SubjDomainAliases::PREVIOUS_SECTION_ARC,
        ScType::NodeVar >> SubjDomainAliases::SECTION_NODE
  );
  previousSectionTemplate.Quintuple(
        SubjDomainAliases::PREVIOUS_SECTION_ARC,
        ScType::EdgeDCommonVar >> SubjDomainAliases::BASE_SEQUENCE_EDGE,
        SubjDomainAliases::SECTION_EDGE,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_basic_sequence
  );
}

}
