/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "identifier_utils.hpp"

namespace verificationModule
{
std::string IdentifierUtils::getIdentifier(ScMemoryContext * context, ScAddr const & element)
{
  std::string identifier = context->GetElementSystemIdentifier(element);

  if (identifier.empty())
    identifier = std::to_string(element.Hash());

  return identifier;
}

}  // namespace verificationModule
