/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sections_builder.hpp"

#include "constants/sections_aliases.hpp"
#include "keynodes/sections_keynodes.hpp"

namespace sectionsModule
{
void sections_builder::buildDecompositionTupleTemplate(
    ScTemplate & scTemplate,
    ScAddr const & parentSection,
    ScAddr const & childSection)
{
  scTemplate.Quintuple(
      parentSection,
      ScType::VarCommonArc,
      ScType::VarNode >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc >> sections_aliases::EDGE_TO_DELETE,
      childSection);
}

void sections_builder::buildDecompositionTupleTemplate(ScTemplate & scTemplate, ScAddr const & section)
{
  scTemplate.Quintuple(
      ScType::VarNode >> sections_aliases::PARENT_SECTION,
      ScType::VarCommonArc,
      ScType::VarNode >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(
      sections_aliases::DECOMPOSITION_TUPLE, ScType::VarPermPosArc >> sections_aliases::EDGE_TO_DELETE, section);
}

void sections_builder::buildNextSectionTemplate(
    ScTemplate & nextSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  nextSectionTemplate.Triple(tuple, ScType::VarPermPosArc >> sections_aliases::SECTION_EDGE, section);
  nextSectionTemplate.Triple(
      tuple,
      ScType::VarPermPosArc >> sections_aliases::NEXT_SECTION_ARC,
      ScType::VarNode >> sections_aliases::SECTION_NODE);
  nextSectionTemplate.Quintuple(
      sections_aliases::SECTION_EDGE,
      ScType::VarCommonArc >> sections_aliases::BASE_SEQUENCE_EDGE,
      sections_aliases::NEXT_SECTION_ARC,
      ScType::VarPermPosArc,
      ScKeynodes::nrel_basic_sequence);
}

void sections_builder::buildPreviousSectionTemplate(
    ScTemplate & previousSectionTemplate,
    ScAddr const & tuple,
    ScAddr const & section)
{
  previousSectionTemplate.Triple(tuple, ScType::VarPermPosArc >> sections_aliases::SECTION_EDGE, section);
  previousSectionTemplate.Triple(
      tuple,
      ScType::VarPermPosArc >> sections_aliases::PREVIOUS_SECTION_ARC,
      ScType::VarNode >> sections_aliases::SECTION_NODE);
  previousSectionTemplate.Quintuple(
      sections_aliases::PREVIOUS_SECTION_ARC,
      ScType::VarCommonArc >> sections_aliases::BASE_SEQUENCE_EDGE,
      sections_aliases::SECTION_EDGE,
      ScType::VarPermPosArc,
      ScKeynodes::nrel_basic_sequence);
}

}  // namespace sectionsModule
