/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */
#pragma once

#include "sc-memory/kpm/sc_agent.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "RemoveSectionAgent.generated.hpp"

namespace subjectDomainModule
{
class RemoveSectionAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkActionClass(ScAddr const & actionNode);

  bool removeSection(ScAddr const & section, ScAddr const & parentSection);

  bool removeSection(ScAddr const & section);

  void handleSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void handleParentSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void handleNeighboringSections(ScAddr const &, ScAddr const &);
};

}  // namespace subjDomainModule
