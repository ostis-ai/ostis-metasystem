/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace verificationModule
{

class VerificationKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_check_duplicate_constructions{
      "action_check_duplicate_constructions",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_duplicate_construction_file{
      "nrel_duplicate_construction_file",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const rrel_not_maximum_studied_object_class{
      "rrel_not_maximum_studied_object_class",
      ScType::ConstNodeRole};
  static inline ScKeynode const rrel_maximum_studied_object_class{
      "rrel_maximum_studied_object_class",
      ScType::ConstNodeRole};
  static inline ScKeynode const lang_en{"lang_en", ScType::ConstNodeClass};
  static inline ScKeynode const concept_singular_relation{"concept_singular_relation", ScType::ConstNodeClass};
  static inline ScKeynode const quasybinary_relation{"quasybinary_relation", ScType::ConstNodeClass};
  static inline ScKeynode const entity_class{"entity_class", ScType::ConstNodeClass};
};

}  // namespace verificationModule
