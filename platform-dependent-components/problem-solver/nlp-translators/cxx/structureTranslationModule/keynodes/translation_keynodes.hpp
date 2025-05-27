/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #pragma once

 #include <sc-memory/sc_keynodes.hpp>
 
 namespace structureTranslationModule
 {
 
 class TranslationKeynodes : public ScKeynodes
 {
 public:
   static inline ScKeynode const action_translate_structures_into_natural_language{
       "action_translate_structures_into_natural_language",
       ScType::ConstNodeClass};
   static inline ScKeynode const lang_en{
       "lang_en",
       ScType::ConstNodeClass};
   static inline ScKeynode const translation_ignored_keynodes{
       "translation_ignored_keynodes",
       ScType::ConstNodeClass};
   static inline ScKeynode const subject_domain{
       "subject_domain",
       ScType::ConstNodeClass};
 };
 
 }  // namespace structureTranslationModule
