/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "subject_domain_keynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
ScAddr SubjectDomainKeynodes::action_get_decomposition;
ScAddr SubjectDomainKeynodes::action_add_section;
ScAddr SubjectDomainKeynodes::action_remove_section;
ScAddr SubjectDomainKeynodes::nrel_section_decomposition;
ScAddr SubjectDomainKeynodes::nrel_entity_decomposition;
ScAddr SubjectDomainKeynodes::removed_section;
ScAddr SubjectDomainKeynodes::section;
ScAddr SubjectDomainKeynodes::not_enough_formed_structure;
ScAddr SubjectDomainKeynodes::atomic_section;
ScAddr SubjectDomainKeynodes::non_atomic_section;
ScAddr SubjectDomainKeynodes::rrel_last;
}  // namespace subjectDomainModule
