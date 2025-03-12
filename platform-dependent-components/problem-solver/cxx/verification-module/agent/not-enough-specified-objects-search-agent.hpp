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
  static void GetElements(ScMemoryContext & m_context, ScAddr const & class_node, ScAddrList & elements);
  static bool CheckTemplate(ScMemoryContext & m_context, ScAddr const & argument, ScAddr const & classNode);

private:
  ScAddrList GetSubjectDomainAddr(ScAddr const & nodeAddr) const;
  void GetNodeSubjectDomainsId(std::stringstream & infoAboutBasicSpec, ScAddr const & nodeAddr) const;
  static bool WriteInFile(std::string const & strIdentifiers);
  static std::filesystem::path filePath;
  void GetNodeSystemId(std::stringstream & infoAboutBasicSpec, ScAddr const & nodeAddr) const;
  std::list<std::pair<ScAddr, ScAddrList>> GetSpecificationTemplateAndElements() const;
};

}  // namespace verificationModule
