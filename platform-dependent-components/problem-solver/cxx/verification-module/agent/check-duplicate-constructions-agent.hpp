/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace verificationModule
{
class CheckDuplicateConstructionsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::string  findDomainSectionIdtf(
      ScAddr & classAddr,
      std::string & stringDomainSection);

  std::ofstream createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr);

  std::vector<ScAddr> createClassObjectsVector(ScAddr & classAddr);

  void checkDuplicateTriplets(
      std::vector<ScAddr> & classObjects,
      std::ofstream & outputFile,
      std::string & stringDomainSection);

  void checkDuplicateFives(
      std::vector<ScAddr> & classObjects,
      std::ofstream & outputFile,
      std::string & stringDomainSection);

  void checkDuplicateQuasyRelations(std::vector<ScAddr> & classObjects, std::ofstream & outputFile);

  std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> fillTripleVector(ScAddr & classObject);

  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fillFivesVector(ScAddr & classObject);

  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fillQuasyRelationVector(ScAddr & classObject);

  int checkDuplicateRelationTriplets(ScAddr & relationObject, ScAddr & classObject);

  void createDuplicateTripletsInfo(
      ScAddr & classObject,
      ScAddr & relationObject,
      std::ofstream & outputFile,
      std::string & stringDomainSection);

  int checkDuplicateRelationFives(ScAddr & relationObject, ScAddr & classObject, ScAddr & relation);

  void createDuplicateFivesInfo(
      ScAddr & classObject,
      ScAddr & relationObject,
      ScAddr & relation,
      std::ofstream & outputFile,
      std::string & stringDomainSection);

  int checkDuplicateRelationQuasyRelations(ScAddr & tupleObject, ScAddr & objectClass, ScAddr & relation);

  void createDuplicateQuasyRelationsInfo(
      ScAddr & classObject,
      ScAddr & relationObject,
      ScAddr & relation,
      std::ofstream & outputFile);
};

}  // namespace verificationModule
