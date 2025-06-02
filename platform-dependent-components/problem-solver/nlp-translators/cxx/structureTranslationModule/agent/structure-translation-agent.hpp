/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #pragma once

 #include <sc-memory/sc_agent.hpp>
 
 
 namespace structureTranslationModule
 {
 class StructureTranslationAgent : public ScActionInitiatedAgent
 {
  public:
   ScAddr GetActionClass() const override;
 
   ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
 
 private:
   std::string translateStructure(ScAddr const & structAddr, ScAddr const & lang, ScMemoryContext * context);
 };
 
 }  // namespace structureTranslationModule
