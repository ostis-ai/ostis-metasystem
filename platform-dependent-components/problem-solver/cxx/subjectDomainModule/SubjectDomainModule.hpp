/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "SubjectDomainModule.generated.hpp"

class SubjectDomainModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  virtual sc_result InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};
