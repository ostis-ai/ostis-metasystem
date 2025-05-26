#pragma once

#include <string>

namespace naturalLanguageProcessingModule
{
class TranslationConstants
{
public:
  static size_t const MAX_TRANSLATIONS_FROM_EACH_TRANSLATOR;
  static size_t const MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS;

  static std::string const TRANSLATIONS_DELIMITER;
};

}  // namespace naturalLanguageProcessingModule
