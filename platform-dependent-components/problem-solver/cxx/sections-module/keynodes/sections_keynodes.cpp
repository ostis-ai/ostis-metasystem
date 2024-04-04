/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sections_keynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace sectionsModule
{
ScAddr SectionsKeynodes::action_get_decomposition;
ScAddr SectionsKeynodes::action_add_section;
ScAddr SectionsKeynodes::action_remove_section;
ScAddr SectionsKeynodes::nrel_section_decomposition;
ScAddr SectionsKeynodes::nrel_entity_decomposition;
ScAddr SectionsKeynodes::removed_section;
ScAddr SectionsKeynodes::section;
ScAddr SectionsKeynodes::not_enough_formed_structure;
ScAddr SectionsKeynodes::atomic_section;
ScAddr SectionsKeynodes::non_atomic_section;
ScAddr SectionsKeynodes::rrel_last;
}  // namespace sectionsModule
