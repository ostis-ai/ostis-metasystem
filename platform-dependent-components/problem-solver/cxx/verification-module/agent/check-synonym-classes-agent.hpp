/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace verificationModule
{
class CheckSynonymClassesAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:

  void checkSynonymFives(
      std::vector<ScAddr> & classObjects,
      std::ofstream & outputFile);

  void createSynonymClassesInfo(
      ScAddr & identifier,
      ScAddr & firstSynonym,
      ScAddr & secondSynonym,
      std::ofstream & outputFile);

  void createClassInfo(
      ScAddr & class,
      std::ofstream & outputFile);

  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fillFivesVector(ScAddr & classObject);

  std::string  findDomainSectionIdtf(ScAddr & object);
    
    std::ofstream openOrCreateFile(
    ScAddr & classAddr,
    std::string const & filename);

    std::ofstream generateSynonymFile(
    ScAddr const & classAddr,
    std::string const & filename);

  std::ofstream createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr);

  std::vector<ScAddr> createXlassObjectsVector(ScAddr & classAddr);

  std::string getDefinition(ScAddr & class);

};

}  // namespace verificationModule
