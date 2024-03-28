/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */
#pragma once

#include "sc-memory/kpm/sc_agent.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "AddSectionAgent.generated.hpp"

namespace subjectDomainModule
{
class AddSectionAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionNode);

  ScAddr generateSection(std::string const & sectionName, ScAddr const & parentSection, ScAddr const & lang);

  ScAddr generateSection(std::string const & sectionName, ScAddr const & lang);
};

}  // namespace subjDomainModule
