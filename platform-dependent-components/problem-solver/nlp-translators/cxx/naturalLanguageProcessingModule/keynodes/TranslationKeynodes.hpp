#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "TranslationKeynodes.generated.hpp"

namespace naturalLanguageProcessingModule
{

class TranslationKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_form_semantic_neighbourhood_translation"), ForceCreate(ScType::NodeConstClass))
  static ScAddr action_form_semantic_neighbourhood_translation;

  SC_PROPERTY(Keynode("lang_en"), ForceCreate(ScType::NodeConstClass))
  static ScAddr lang_en;

  SC_PROPERTY(Keynode("parameter"), ForceCreate(ScType::NodeConstClass))
  static ScAddr parameter;

  SC_PROPERTY(Keynode("translation_ignored_keynodes"), ForceCreate(ScType::NodeConstClass))
  static ScAddr translation_ignored_keynodes;
};

}  // namespace naturalLanguageProcessingModule
