/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "identifiers_keynodes.generated.hpp"

namespace identifiersModule
{
class IdentifiersKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_find_identifiers"), ForceCreate)
  static ScAddr action_find_identifiers;

};

}  // namespace identifiersModule
