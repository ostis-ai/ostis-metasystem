/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/kpm/sc_agent.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "generated/remove_section_agent.generated.hpp"

namespace sectionsModule
{
class RemoveSectionAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool CheckActionClass(ScAddr const & actionNode);

  bool RemoveSection(ScAddr const & section, ScAddr const & parentSection);

  bool RemoveSection(ScAddr const & section);

  void HandleSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void HandleParentSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void HandleNeighboringSections(ScAddr const &, ScAddr const &);
};

}  // namespace subjectDomainModule
