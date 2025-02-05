/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

#include "filesystem"

namespace verificationModule
{
class SearchNotEnoughSpecifiedObjectsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;
  std::map<ScType, ScAddr> scTypeAndActionClass{
      {ScType::ConstNodeClass, VerificationKeynodes::action_search_not_enough_specified_classes},
      {ScType::ConstNodeNonRole, VerificationKeynodes::action_search_not_enough_specified_relations},
      {ScType::ConstNodeRole, VerificationKeynodes::action_search_not_enough_specified_relations},
      {ScType::ConstNodeStructure, VerificationKeynodes::action_search_not_enough_specified_structures}};
  ScResult DoProgram(ScAction & action) override;
  static ScAddrVector GetElements(ScMemoryContext & m_context, ScAddr const & class_node);
  static ScAddr GetTemplateAddr(ScMemoryContext & m_context, ScAddr const & classNode);
  static bool CheckTemplate(ScMemoryContext & m_context, ScAddr const & argument, ScAddr const & classNode);


private:
  ScAddrVector GetSubjectDomainAddr(ScAddr const & nodeAddr) const;
  void getInfoAboutNode(std::stringstream & infoAboutBasicSpec, ScAddr const & nodeAddr) const;
  bool WriteInFile(std::string const & strIdentifiers);
  static std::filesystem::path filePath;
};

}  // namespace verificationModule
