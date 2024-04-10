/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/kpm/sc_agent.hpp"

#include <nlohmann/json.hpp>

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "generated/get_decomposition_agent.generated.hpp"

using json = nlohmann::json;

namespace sectionsModule
{
class GetDecompositionAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool CheckActionClass(ScAddr const & actionNode);

  json GetJSONDecomposition(ScAddrVector const &, size_t, ScAddr const &, ScAddr const &);

  ScAddrVector GetDecomposition(ScAddr const &, ScAddr const &);
};

}  // namespace sectionsModule
