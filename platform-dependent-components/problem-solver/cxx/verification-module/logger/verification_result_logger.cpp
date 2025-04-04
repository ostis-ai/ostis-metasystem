/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_result_logger.hpp"

namespace verificationModule
{
void VerificationResultLogger::logSetCheckResult(SetCheckResult const & checkResult, std::ofstream & logFile)
{
  logSetInfo(checkResult, logFile);
  for (auto const & elementCheckResult : checkResult.elemtnsCheckResults)
    logElementCheckResult(elementCheckResult, logFile);
}

void VerificationResultLogger::logSetInfo(SetCheckResult const & checkResult, std::ofstream & logFile)
{
  std::string description =
      "Check results for " + checkResult.setIdtf + ". Check performed at " + checkResult.checkTime + ".\n";

  if (!checkResult.subjectDomainContainingAsMaximumClass.empty())
    description += "Checked set belong to " + checkResult.subjectDomainContainingAsMaximumClass
                   + " as maximum studied object class.\n";

  if (!checkResult.subjectDomainsContainingAsNotMaximumClass.empty())
  {
    std::string subjectDomainsEnumeration;
    for (auto const & subjectDomain : checkResult.subjectDomainsContainingAsNotMaximumClass)
      subjectDomainsEnumeration += subjectDomain + ", ";
    subjectDomainsEnumeration.erase(subjectDomainsEnumeration.length() - 2);  // remove ", " after the last element

    description += "Checked set belong to " + subjectDomainsEnumeration + " as not maximum studied object classes.\n";
  }

  logFile << description;
}

void VerificationResultLogger::logElementCheckResult(ElementCheckResult const & checkResult, std::ofstream & logFile)
{
  if (!checkResult.errorsDescriptions.empty())
  {
    logFile << "Found next errors in checked set element " << checkResult.elementIdtf << ":\n";
    for (auto const & errorDescription : checkResult.errorsDescriptions)
      logFile << INDENT << "- " << errorDescription << "\n";
  }

  if (!checkResult.warningDescriptions.empty())
  {
    logFile << "Found next possible errors in checked set element " << checkResult.elementIdtf << ":\n";
    for (auto const & warningDescription : checkResult.warningDescriptions)
      logFile << INDENT << "- " << warningDescription << "\n";
  }
}

}  // namespace verificationModule
