/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "Keynodes.generated.hpp"

namespace generateResponseModule
{
class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_generate_response"), ForceCreate)
  static ScAddr action_generate_response;

  SC_PROPERTY(Keynode("nrel_response_action"), ForceCreate)
  static ScAddr nrel_response_action;

  SC_PROPERTY(Keynode("concept_entity_possible_class"), ForceCreate)
  static ScAddr concept_entity_possible_class;

  SC_PROPERTY(Keynode("concept_intent_possible_class"), ForceCreate)
  static ScAddr concept_intent_possible_class;

  SC_PROPERTY(Keynode("nrel_entity_possible_role"), ForceCreate)
  static ScAddr nrel_entity_possible_role;

  SC_PROPERTY(Keynode("rrel_entity"), ForceCreate)
  static ScAddr rrel_entity;

  SC_PROPERTY(Keynode("nrel_reply_structure"), ForceCreate)
  static ScAddr nrel_reply_structure;
};

}  // namespace generateResponseModule
