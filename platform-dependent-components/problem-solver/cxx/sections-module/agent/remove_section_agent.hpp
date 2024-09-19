/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace sectionsModule
{
class RemoveSectionAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  bool RemoveSection(ScAddr const & section, ScAddr const & parentSection);

  bool RemoveSection(ScAddr const & section);

  void HandleSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void HandleParentSection(ScTemplateSearchResultItem const &, ScAddr const &);

  void HandleNeighboringSections(ScAddr const &, ScAddr const &);
};

}  // namespace sectionsModule
