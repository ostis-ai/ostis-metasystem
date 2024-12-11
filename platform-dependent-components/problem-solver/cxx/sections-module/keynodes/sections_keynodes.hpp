/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace sectionsModule
{
class SectionsKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_get_decomposition{"action_get_decomposition", ScType::ConstNodeClass};

  static inline ScKeynode const action_add_section{"action_add_section", ScType::ConstNodeClass};

  static inline ScKeynode const action_remove_section{"action_remove_section", ScType::ConstNodeClass};

  static inline ScKeynode const nrel_section_decomposition{"nrel_section_decomposition", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_entity_decomposition{"nrel_entity_decomposition", ScType::ConstNodeNonRole};

  static inline ScKeynode const removed_section{"removed_section", ScType::ConstNodeClass};

  static inline ScKeynode const section{"section", ScType::ConstNodeClass};

  static inline ScKeynode const not_enough_formed_structure{"not_enough_formed_structure", ScType::ConstNode};

  static inline ScKeynode const atomic_section{"atomic_section", ScType::ConstNode};

  static inline ScKeynode const non_atomic_section{"non_atomic_section", ScType::ConstNode};

  static inline ScKeynode const rrel_last{"rrel_last", ScType::ConstNodeRole};

  static inline ScKeynode const nrel_answer{"nrel_answer", ScType::ConstNodeNonRole};
};

}  // namespace sectionsModule
