/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "generated/subject_domain_keynodes.generated.hpp"

namespace subjectDomainModule
{
class subject_domain_keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_get_decomposition"), ForceCreate)
  static ScAddr action_get_decomposition;

  SC_PROPERTY(Keynode("action_add_section"), ForceCreate)
  static ScAddr action_add_section;

  SC_PROPERTY(Keynode("action_remove_section"), ForceCreate)
  static ScAddr action_remove_section;

  SC_PROPERTY(Keynode("nrel_section_decomposition"), ForceCreate)
  static ScAddr nrel_section_decomposition;

  SC_PROPERTY(Keynode("nrel_entity_decomposition"), ForceCreate)
  static ScAddr nrel_entity_decomposition;

  SC_PROPERTY(Keynode("removed_section"), ForceCreate)
  static ScAddr removed_section;

  SC_PROPERTY(Keynode("section"), ForceCreate)
  static ScAddr section;

  SC_PROPERTY(Keynode("not_enough_formed_structure"), ForceCreate)
  static ScAddr not_enough_formed_structure;

  SC_PROPERTY(Keynode("atomic_section"), ForceCreate)
  static ScAddr atomic_section;

  SC_PROPERTY(Keynode("non_atomic_section"), ForceCreate)
  static ScAddr non_atomic_section;

  SC_PROPERTY(Keynode("rrel_last"), ForceCreate)
  static ScAddr rrel_last;
};

}  // namespace subjectDomainModule
