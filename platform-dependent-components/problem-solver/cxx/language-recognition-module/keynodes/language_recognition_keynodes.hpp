/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace languageRecognitionModule
{

class LanguageRecognitionKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_language_recognition{
      "action_language_recognition",
      ScType::ConstNodeClass};
  static inline ScKeynode const lang_en{
      "lang_en",
      ScType::ConstNodeClass};
  static inline ScKeynode const nrel_sc_text_translation{
      "nrel_sc_text_translation",
      ScType::ConstNodeRole};
};

}  // namespace languageRecognitionModule
