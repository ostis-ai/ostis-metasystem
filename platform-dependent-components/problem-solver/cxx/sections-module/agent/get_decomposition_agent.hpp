/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <nlohmann/json.hpp>

#include <sc-memory/sc_agent.hpp>

using json = nlohmann::json;

namespace sectionsModule
{
class GetDecompositionAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;

private:
  json GetJSONDecomposition(ScAddrVector const &, size_t, ScAddr const &, ScAddr const &);

  ScAddrVector GetDecomposition(ScAddr const &, ScAddr const &);
};

}  // namespace sectionsModule
