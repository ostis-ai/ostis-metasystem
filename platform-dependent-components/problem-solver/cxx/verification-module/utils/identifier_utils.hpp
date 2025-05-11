/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include <sc-memory/sc_memory.hpp>

namespace verificationModule
{

class IdentifierUtils
{
public:
  static std::string GetUniqueIdentifier(ScMemoryContext * context, ScAddr const & element);

  static std::string GetIdentifiersString(ScMemoryContext * context, ScAddr const & element);

private:
  static void GetMainIdentifiers(
      ScMemoryContext * context,
      ScAddr const & element,
      std::list<std::string> & identifiers);
};

}  // namespace verificationModule
