/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace searchAnswerModule
{

class SearchAnswerKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_search_properties{
      "action_search_properties",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_properties{
      "nrel_properties",
      ScType::ConstNodeNonRole};
  static inline ScKeynode const concept_property{
      "concept_property",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_log{
      "nrel_log",
      ScType::ConstNodeRole};
  static inline ScKeynode const nrel_addition{
      "nrel_addition",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_substraction{
      "nrel_substraction",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_multiplication{
      "nrel_multiplication",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_division{
      "nrel_division",
      ScType::ConstNodeClass};
static inline ScKeynode const lang_en{
    "lang_en",
    ScType::ConstNodeClass};
};

}  // namespace verificationModule
