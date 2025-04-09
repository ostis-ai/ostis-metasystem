/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #pragma once

 #include <sc-memory/sc_keynodes.hpp>
 
 namespace messageReplyModule
 {
 
 class MessageReplyKeynodes : public ScKeynodes
 {
 public:

   static inline ScKeynode const action_reply_to_message{
       "action_reply_to_message",
       ScType::ConstNodeClass};
   static inline ScKeynode const nrel_authors{
       "nrel_authors",
       ScType::ConstNodeNonRole};
   static inline ScKeynode const nrel_reply{
       "nrel_reply",
       ScType::ConstNodeNonRole};
   static inline ScKeynode const action_find_identifiers{
       "action_find_identifiers",
       ScType::ConstNodeClass};
   static inline ScKeynode const message_processing_program{
       "message_processing_program",
       ScType::ConstNodeClass};
   static inline ScKeynode const languages{
       "languages",
       ScType::ConstNodeClass};
   static inline ScKeynode const concept_text_file{
       "concept_text_file",
       ScType::ConstNodeClass};
   static inline ScKeynode const concept_message{
       "concept_message",
       ScType::ConstNodeClass};
   static inline ScKeynode const action_interpret_non_atomic_action{
       "action_interpret_non_atomic_action",
       ScType::ConstNodeClass};
   static inline ScKeynode const nrel_sc_text_translation{
        "nrel_sc_text_translation",
        ScType::ConstNodeNonRole};
 };
 
 }  // namespace messageReplyModule

