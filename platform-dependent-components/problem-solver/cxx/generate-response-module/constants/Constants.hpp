/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace generateResponseModule
{
class Constants : public ScObject
{
public:
  static std::string const generateAnswerAgentClassName;

  static std::string const messageAddrParamName;

  static std::string const answerAddrParamName;
};

}  // namespace generateResponseModule
