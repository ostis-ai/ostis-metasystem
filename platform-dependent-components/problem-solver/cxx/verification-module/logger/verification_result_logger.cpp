/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_result_logger.hpp"

namespace verificationModule
{
void VerificationResultLogger::logSetCheckResult(
    SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const
{
  logSetInfo(checkResult, logFile);
  for (auto const & elementCheckResult : checkResult.elemtnsCheckResults)
    logElementCheckResult(elementCheckResult, logFile);
}

void VerificationResultLogger::logSetInfo(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const
{
  std::stringstream descriptionStream;
  descriptionStream << "Check results for " << checkResult.setIdtf
                    << ". Check performed at " << checkResult.checkTime << ".\n";

  descriptionStream << "Checked set belong to ";
  addEnumeration(checkResult.subjectDomainsContainingAsMaximumClass, descriptionStream);
  descriptionStream << " as maximum studied object class.\n";

  descriptionStream << "Checked set belong to ";
  addEnumeration(checkResult.subjectDomainsContainingAsNotMaximumClass, descriptionStream);
  descriptionStream << " as not maximum studied object class.\n";

  logFile << descriptionStream.rdbuf();
}

void VerificationResultLogger::logElementCheckResult(
    ElementDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const
{
  std::stringstream errorsStream;
  if (!checkResult.errorsDescriptions.empty())
  {
    errorsStream << "Found next errors in checked set element " << checkResult.elementIdtf << ":\n";
    for (auto const & errorDescription : checkResult.errorsDescriptions)
      errorsStream << INDENT << "- " << errorDescription << "\n";
  }

  if (!checkResult.warningDescriptions.empty())
  {
    errorsStream << "Found next possible errors in checked set element " << checkResult.elementIdtf << ":\n";
    for (auto const & warningDescription : checkResult.warningDescriptions)
      errorsStream << INDENT << "- " << warningDescription << "\n";
  }

  logFile << errorsStream.rdbuf();
}

void VerificationResultLogger::addEnumeration(
    std::list<std::string> const & enumerationElements, std::stringstream & stringStream) const
{
  ;
  bool first = true;
  for (auto const & subjectDomain : enumerationElements)
  {
    if (!first)
      stringStream << ", ";

    stringStream << subjectDomain;

    first = false;
  }
}

}  // namespace verificationModule
