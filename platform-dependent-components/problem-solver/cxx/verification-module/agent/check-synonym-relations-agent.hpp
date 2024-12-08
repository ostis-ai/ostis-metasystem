/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace verificationModule
{
class CheckSynonymObjectsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:

  void checkSynonymFives(
      std::vector<ScAddr> & classObjects,
      std::ofstream & outputFile);

  void processSynonymObjects(
      ScAddr & identifier,
      ScAddr & firstSynonym,
      ScAddr & secondSynonym);

  void createSynonymRelationsInfo(
      ScAddr & relation,
      ScAddr & firstFirstDomain,
      std::ofstream & firstSecondDomain,
      ScAddr & secondFirstDomain,
      std::ofstream & secondSecondDomain,
      std::string & domainSectionIdtf);
      
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fillFivesVector(ScAddr & classObject);

  std::string  findDomainSectionIdtf(ScAddr & classAddr);
    
    std::ofstream openOrCreateFile(
    ScAddr & classAddr,
    std::string const & filename);

    std::ofstream generateSynonymFile(
    ScAddr const & classAddr,
    std::string const & filename);

  std::ofstream createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr);

  std::vector<ScAddr> createRelationObjectsVector(ScAddr & classAddr);

};

}  // namespace verificationModule
