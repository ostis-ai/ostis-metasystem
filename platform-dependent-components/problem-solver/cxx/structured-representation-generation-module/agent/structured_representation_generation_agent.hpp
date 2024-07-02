/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "manager/structured_representation_generation_manager.hpp"

#include "structured_representation_generation_agent.generated.hpp"

namespace structuredRepresentationGenerationModule 
{
class StructuredRepresentationGenerationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  std::unique_ptr<StructuredRepresentationGenerationManager> manager;

  bool CheckActionClass(ScAddr const & actionNode);

  void InitFields();

  static std::string GetClassNameForLog();
};

}
