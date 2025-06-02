/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace messageClassificationModule
{

class MessageKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_phrase_generation{
      "action_phrase_generation",
      ScType::ConstNodeClass};
  static inline ScKeynode const action_standard_message_reply{
      "action_standard_message_reply",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_answer_on_standard_message_rule{
      "concept_answer_on_standard_message_rule",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_answer_on_standard_message_rule_class_by_priority{
      "concept_answer_on_standard_message_rule_class_by_priority",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_atomic_message{
      "concept_atomic_message",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_non_atomic_message{
      "concept_non_atomic_message",
      ScType::ConstNodeClass};
  static inline ScKeynode const concept_message{
      "concept_message",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_answer_pattern{
      "nrel_answer_pattern",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_authors{
      "nrel_authors",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_message_key_elements{
      "nrel_message_key_elements",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_message_decomposition{
      "nrel_message_decomposition",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_message_sequence{
    "nrel_message_sequence",
    ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_reply{
      "nrel_reply",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const rrel_message_theme{
      "rrel_message_theme",
      ScType::ConstNodeRole};
  static inline ScKeynode const concept_text_file{
      "concept_text_file",
      ScType::ConstNodeClass};
  static inline ScKeynode const answer_structure{
      "answer_structure",
      ScType::ConstNode};
  static inline ScKeynode const template_reply_target{
      "template_reply_target",
      ScType::ConstNodeClass};
  static inline ScKeynode const rrel_entity{
      "rrel_entity",
      ScType::ConstNodeRole};
  static inline ScKeynode const nrel_semantic_equivalent{
      "nrel_semantic_equivalent",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_sc_text_translation{
      "nrel_sc_text_translation",
       ScType::ConstNodeNonRole};
      
};

}  // namespace messageClassificationModule
