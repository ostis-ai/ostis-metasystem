/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "keynodes/verification_keynodes.hpp"

#include "identifier_utils.hpp"

namespace verificationModule
{
std::string IdentifierUtils::GetUniqueIdentifier(ScMemoryContext * context, ScAddr const & element)
{
  std::string identifier = context->GetElementSystemIdentifier(element);

  if (identifier.empty())
    identifier = std::to_string(element.Hash());

  return identifier;
}

std::string IdentifierUtils::GetIdentifiersString(ScMemoryContext * context, ScAddr const & element)
{
  std::string uniqueIdentifier = GetUniqueIdentifier(context, element);

  std::list<std::string> mainIdentifiers;
  GetMainIdentifiers(context, element, mainIdentifiers);

  if (mainIdentifiers.empty())
    return uniqueIdentifier;

  std::string identifiersString = uniqueIdentifier + " (";
  for (auto const & mainIdentifier : mainIdentifiers)
  {
    identifiersString += mainIdentifier;
    identifiersString += ", ";
  }
  identifiersString.erase(identifiersString.length() - 2);  // remove ", " after the last element
  identifiersString += ")";

  return identifiersString;
}

void IdentifierUtils::GetMainIdentifiers(
    ScMemoryContext * context,
    ScAddr const & element,
    std::list<std::string> & identifiers)
{
  ScIterator5Ptr identifiersIterator = context->CreateIterator5(
      element,
      ScType::ConstCommonArc,
      ScType::ConstNodeLink,
      ScType::ConstPermPosArc,
      VerificationKeynodes::nrel_main_idtf);

  std::string identifierString;
  while (identifiersIterator->Next())
  {
    context->GetLinkContent(identifiersIterator->Get(2), identifierString);
    identifiers.push_back(identifierString);
  }
}

}  // namespace verificationModule
