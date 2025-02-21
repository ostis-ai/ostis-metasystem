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
  ScResult DoProgram(ScAction & elementAddr) override;
  static ScAddrVector GetElements(ScMemoryContext & m_context, ScAddr const & class_node);
  static bool CheckTemplate(ScMemoryContext & m_context, ScAddr const & argument, ScAddr const & classNode);

private:
  ScAddrVector GetSubjectDomainAddr(ScAddr const & nodeAddr) const;
  void getNodeSubjectDomainsId(std::stringstream & infoAboutBasicSpec, ScAddr const & nodeAddr) const;
  static bool WriteInFile(std::string const & strIdentifiers);
  static std::filesystem::path filePath;
  void getNodeSystemId(std::stringstream & infoAboutBasicSpec, ScAddr const & nodeAddr) const;
  std::vector<std::pair<ScAddr, ScAddrVector>> GetSpecificationTemplateAndElements() const;
};

}  // namespace verificationModule
