/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_result_logger.hpp"

namespace verificationModule
{
void VerificationResultLogger::LogSetCheckResult(
    SetDuplicationsCheckResult const & checkResult,
    std::ofstream & logFile) const
{
  LogSetInfo(checkResult, logFile);
  for (auto const & elementCheckResult : checkResult.elemtnsCheckResults)
    LogElementCheckResult(elementCheckResult, logFile);
}

void VerificationResultLogger::LogSetInfo(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const
{
  std::stringstream descriptionStream;
  descriptionStream << "Check results for " << checkResult.setIdtf << ". Check performed at " << checkResult.checkTime
                    << ".\n";

  if (!checkResult.subjectDomainsContainingAsMaximumClass.empty())
  {
    descriptionStream << "Checked set belong to ";
    AddEnumeration(checkResult.subjectDomainsContainingAsMaximumClass, descriptionStream);
    descriptionStream << " as maximum studied object class.\n";
  }

  if (!checkResult.subjectDomainsContainingAsNotMaximumClass.empty())
  {
    descriptionStream << "Checked set belong to ";
    AddEnumeration(checkResult.subjectDomainsContainingAsNotMaximumClass, descriptionStream);
    descriptionStream << " as not maximum studied object class.\n";
  }

  logFile << descriptionStream.rdbuf();
}

void VerificationResultLogger::LogElementCheckResult(
    ElementDuplicationsCheckResult const & checkResult,
    std::ofstream & logFile) const
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

void VerificationResultLogger::AddEnumeration(
    std::list<std::string> const & enumerationElements,
    std::stringstream & stringStream) const
{
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
