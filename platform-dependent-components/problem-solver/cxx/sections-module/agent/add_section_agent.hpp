/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace sectionsModule
{
class AddSectionAgent : public ScActionInitiatedAgent
{
public:
  AddSectionAgent();

  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;

private:
  ScAddr GenerateSection(std::string const & sectionName, ScAddr const & parentSection, ScAddr const & lang);

  ScAddr GenerateSection(std::string const & sectionName, ScAddr const & lang);
};

}  // namespace sectionsModule
