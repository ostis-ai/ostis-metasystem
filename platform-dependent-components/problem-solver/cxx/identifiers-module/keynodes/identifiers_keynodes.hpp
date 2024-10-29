/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace identifiersModule
{

class IdentifiersKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_find_identifiers{"action_find_identifiers", ScType::ConstNodeClass};
};

}  // namespace identifiersModule
