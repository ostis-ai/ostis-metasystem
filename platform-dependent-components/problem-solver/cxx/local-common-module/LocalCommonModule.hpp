#pragma once

#include "sc-memory/sc_module.hpp"

#include "LocalCommonModule.generated.hpp"

namespace commonModule
{
class LocalCommonModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  sc_result InitializeImpl() override;

  sc_result ShutdownImpl() override;
};

}  // namespace commonModule
