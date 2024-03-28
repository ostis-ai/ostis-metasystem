/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#include "Keynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
ScAddr Keynodes::action_get_decomposition;
ScAddr Keynodes::action_add_section;
ScAddr Keynodes::action_remove_section;
ScAddr Keynodes::nrel_entity_decomposition;
ScAddr Keynodes::removed_section;
ScAddr Keynodes::section;
ScAddr Keynodes::not_enough_formed_structure;
ScAddr Keynodes::atomic_section;
ScAddr Keynodes::non_atomic_section;
ScAddr Keynodes::rrel_last;
}  // namespace subjectDomainModule
