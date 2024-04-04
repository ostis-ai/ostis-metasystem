/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "constants/sections_aliases.hpp"
#include "keynodes/sections_keynodes.hpp"

#include "sections_builder.hpp"

namespace sectionsModule
{
void sections_builder::buildDecompositionTupleTemplate(
    ScTemplate & scTemplate,
    ScAddr const & parentSection,
    ScAddr const & childSection)
{
  scTemplate.Quintuple(
      parentSection,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm >> sections_aliases::EDGE_TO_DELETE,
      childSection);
}

void sections_builder::buildDecompositionTupleTemplate(ScTemplate & scTemplate, ScAddr const & section)
{
  scTemplate.Quintuple(
      ScType::NodeVar >> sections_aliases::PARENT_SECTION,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      sections_aliases::DECOMPOSITION_TUPLE, ScType::EdgeAccessVarPosPerm >> sections_aliases::EDGE_TO_DELETE, section);
}

void sections_builder::buildNextSectionTemplate(
    ScTemplate & nextSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  nextSectionTemplate.Triple(tuple, ScType::EdgeAccessVarPosPerm >> sections_aliases::SECTION_EDGE, section);
  nextSectionTemplate.Triple(
      tuple,
      ScType::EdgeAccessVarPosPerm >> sections_aliases::NEXT_SECTION_ARC,
      ScType::NodeVar >> sections_aliases::SECTION_NODE);
  nextSectionTemplate.Quintuple(
      sections_aliases::SECTION_EDGE,
      ScType::EdgeDCommonVar >> sections_aliases::BASE_SEQUENCE_EDGE,
      sections_aliases::NEXT_SECTION_ARC,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_basic_sequence);
}

void sections_builder::buildPreviousSectionTemplate(
    ScTemplate & previousSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  previousSectionTemplate.Triple(tuple, ScType::EdgeAccessVarPosPerm >> sections_aliases::SECTION_EDGE, section);
  previousSectionTemplate.Triple(
      tuple,
      ScType::EdgeAccessVarPosPerm >> sections_aliases::PREVIOUS_SECTION_ARC,
      ScType::NodeVar >> sections_aliases::SECTION_NODE);
  previousSectionTemplate.Quintuple(
      sections_aliases::PREVIOUS_SECTION_ARC,
      ScType::EdgeDCommonVar >> sections_aliases::BASE_SEQUENCE_EDGE,
      sections_aliases::SECTION_EDGE,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_basic_sequence);
}

}  // namespace sectionsModule
