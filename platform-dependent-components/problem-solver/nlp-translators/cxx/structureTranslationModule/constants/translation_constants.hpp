/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

namespace structureTranslationModule
{
class TranslationConstants
{
public:
  static std::string const SOURCE_ALIAS;
  static std::string const CLASS_ALIAS;
  static std::string const TUPLE_ALIAS;
  static std::string const NODE_ALIAS;
  static std::string const EDGE_ALIAS;
  static std::string const NREL_ALIAS;
  static std::string const LINK_ALIAS;
  static std::string const SEPARATOR;
  static int const MAX_LISTING_COUNT;
};

}  // namespace structureTranslationModule
