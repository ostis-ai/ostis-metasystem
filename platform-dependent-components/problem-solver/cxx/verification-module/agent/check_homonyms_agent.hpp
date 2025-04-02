/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>
#include <filesystem>

namespace verificationModule
{
class CheckHomonymsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  void searchIdentifiers(
      ScAddr const & identifierRelation,
      std::unordered_map<std::string, ScAddrUnorderedSet> & identifiers) const;

  void fillSynonymsInfo(
    std::stringstream & synonymsInfo,
    std::unordered_map<std::string, ScAddrUnorderedSet> const & identifiers) const;

  void fillSubjectDomainInfo(std::stringstream & synonymsInfo, ScAddr const & node) const;

  void fillClassesInfo(std::stringstream & synonymsInfo, ScAddr const & node) const;

  void fillNodeInfo(std::stringstream & synonymsInfo, ScAddr const & node) const;

    std::ofstream generateSynonymFile(
    ScAddr const & classAddr,
    std::string const & filename);

};

}  // namespace verificationModule
