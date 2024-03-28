/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "subject_domain_keynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
ScAddr subject_domain_keynodes::action_get_decomposition;
ScAddr subject_domain_keynodes::action_add_section;
ScAddr subject_domain_keynodes::action_remove_section;
ScAddr subject_domain_keynodes::nrel_entity_decomposition;
ScAddr subject_domain_keynodes::removed_section;
ScAddr subject_domain_keynodes::section;
ScAddr subject_domain_keynodes::not_enough_formed_structure;
ScAddr subject_domain_keynodes::atomic_section;
ScAddr subject_domain_keynodes::non_atomic_section;
ScAddr subject_domain_keynodes::rrel_last;
}  // namespace subjectDomainModule
