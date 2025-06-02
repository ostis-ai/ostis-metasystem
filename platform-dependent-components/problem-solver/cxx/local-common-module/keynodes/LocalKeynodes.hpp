#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace commonModule
{

class LocalKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const lang_en{
      "lang_en",
      ScType::ConstNodeClass};
};

}  // namespace commonModule
