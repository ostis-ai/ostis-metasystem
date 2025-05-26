/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageClassificationModule
{

class MessageClassificationKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_message_topic_classification{
      "action_message_topic_classification",
      ScType::ConstNodeClass};
  static inline ScKeynode const action_alternative_message_topic_classification{
      "action_alternative_message_topic_classification",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_intent_possible_class{
      "concept_intent_possible_class",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_trait_possible_class{
      "concept_trait_possible_class",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_entity_possible_class{
      "concept_entity_possible_class",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_wit_ai_idtf{
      "nrel_wit_ai_idtf",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_entity_possible_role{
      "nrel_entity_possible_role",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const concept_not_classified_by_intent_message{
      "concept_not_classified_by_intent_message",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_not_classified_by_trait_message{
      "concept_not_classified_by_trait_message",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_classify_message_rule{
      "concept_classify_message_rule",
      ScType::ConstNodeClass};
};

}  // namespace messageClassificationModule
