/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #pragma once

 #include <sc-memory/sc_keynodes.hpp>
 
 namespace generateResponseModule
 {
 
 class GenerateResponseKeynodes : public ScKeynodes
 {
 public:
   static inline ScKeynode const action_generate_response{
       "action_generate_response",
       ScType::ConstNodeClass};
   static inline ScKeynode const nrel_response_action{
       "nrel_response_action",
       ScType::ConstNodeNonRole};
   static inline ScKeynode const concept_entity_possible_class{
       "concept_entity_possible_class",
       ScType::ConstNodeClass};
   static inline ScKeynode const concept_intent_possible_class{
       "concept_intent_possible_class",
       ScType::ConstNodeClass};
   static inline ScKeynode const nrel_entity_possible_role{
       "nrel_entity_possible_role",
       ScType::ConstNodeNonRole};
   static inline ScKeynode const rrel_entity{
       "rrel_entity",
       ScType::ConstNodeRole};
   static inline ScKeynode const nrel_reply_structure{
       "nrel_reply_structure",
       ScType::ConstNodeNonRole};
 };
 
 }  // namespace generateResponseModule
