/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "structured_representation_generation_keynodes.generated.hpp"

namespace structuredRepresentationGenerationModule
{

class StructuredRepresentationGenerationKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_generate_structured_representation"), ForceCreate)
  inline static ScAddr action_generate_structured_representation;

  SC_PROPERTY(Keynode("action_search_rule_for_generation"), ForceCreate)
  inline static ScAddr action_search_rule_for_generation;

  SC_PROPERTY(Keynode("rrel_last"), ForceCreate)
  inline static ScAddr rrel_last;

  SC_PROPERTY(Keynode("nrel_generation_rule"), ForceCreate)
  inline static ScAddr nrel_generation_rule;
  
};

}
