/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "constants/subject_domain_aliases.hpp"
#include "keynodes/subject_domain_keynodes.hpp"

#include "subject_domain_builder.hpp"

namespace subjectDomainModule
{
void subject_domain_builder::buildDecompositionTupleTemplate(
    ScTemplate & scTemplate,
    ScAddr const & parentSection,
    ScAddr const & childSection)
{
  scTemplate.Quintuple(
      ScType::NodeVar >> subject_domain_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      parentSection,
      ScType::EdgeAccessVarPosPerm,
      subject_domain_keynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      subject_domain_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::EDGE_TO_DELETE,
      childSection);
}

void subject_domain_builder::buildDecompositionTupleTemplate(ScTemplate & scTemplate, ScAddr const & section)
{
  scTemplate.Quintuple(
      ScType::NodeVar >> subject_domain_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> subject_domain_aliases::PARENT_SECTION,
      ScType::EdgeAccessVarPosPerm,
      subject_domain_keynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      subject_domain_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::EDGE_TO_DELETE,
      section);
}

void subject_domain_builder::buildNextSectionTemplate(
    ScTemplate & nextSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  nextSectionTemplate.Triple(tuple, ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::SECTION_EDGE, section);
  nextSectionTemplate.Triple(
      tuple,
      ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::NEXT_SECTION_ARC,
      ScType::NodeVar >> subject_domain_aliases::SECTION_NODE);
  nextSectionTemplate.Quintuple(
      subject_domain_aliases::SECTION_EDGE,
      ScType::EdgeDCommonVar >> subject_domain_aliases::BASE_SEQUENCE_EDGE,
      subject_domain_aliases::NEXT_SECTION_ARC,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_basic_sequence);
}

void subject_domain_builder::buildPreviousSectionTemplate(
    ScTemplate & previousSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  previousSectionTemplate.Triple(tuple, ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::SECTION_EDGE, section);
  previousSectionTemplate.Triple(
      tuple,
      ScType::EdgeAccessVarPosPerm >> subject_domain_aliases::PREVIOUS_SECTION_ARC,
      ScType::NodeVar >> subject_domain_aliases::SECTION_NODE);
  previousSectionTemplate.Quintuple(
      subject_domain_aliases::PREVIOUS_SECTION_ARC,
      ScType::EdgeDCommonVar >> subject_domain_aliases::BASE_SEQUENCE_EDGE,
      subject_domain_aliases::SECTION_EDGE,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_basic_sequence);
}

}  // namespace subjectDomainModule
