/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "FindIdentifiersAgent.generated.hpp"

namespace identifiersModule
{
class FindIdentifiersAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

  std::map<ScType, std::string> nodes = {

      {ScType::Const, "node_const"},
      {ScType::Var, "node_var"},

      {ScType::Node, "node"},
      {ScType::Link, "link"},
      {ScType::LinkClass, "link_class"},
      {ScType::Unknown, "unknown"},

      {ScType::NodeConst, "node_const"},
      {ScType::NodeVar, "node_var"},

      {ScType::LinkConst, "link_const"},
      {ScType::LinkVar, "link_var"},

      {ScType::NodeStruct, "node_struct"},
      {ScType::NodeTuple, "node_tuple"},
      {ScType::NodeRole, "node_role"},
      {ScType::NodeNoRole, "node_no_role"},
      {ScType::NodeClass, "node_class"},
      {ScType::NodeAbstract, "node_abstract"},
      {ScType::NodeMaterial, "node_material"},

      {ScType::NodeConstStruct, "node_const_struct"},
      {ScType::NodeConstTuple, "node_const_tuple"},
      {ScType::NodeConstRole, "node_const_role"},
      {ScType::NodeConstNoRole, "node_const_no_role"},
      {ScType::NodeConstClass, "node_const_class"},
      {ScType::NodeConstAbstract, "node_const_abstract"},
      {ScType::NodeConstMaterial, "node_const_material"},

      {ScType::NodeVarStruct, "node_var_struct"},
      {ScType::NodeVarTuple, "node_var_tuple"},
      {ScType::NodeVarRole, "node_var_tuple"},
      {ScType::NodeVarNoRole, "node_var_no_role"},
      {ScType::NodeVarClass, "node_var_class"},
      {ScType::NodeVarAbstract, "node_var_abstract"},
      {ScType::NodeVarMaterial, "node_var_material"}};

  std::map<ScType, std::string> edges = {
      {ScType::EdgeUCommon, "edge_ucommon"},
      {ScType::EdgeDCommon, "edge_dcommon"},

      {ScType::EdgeUCommonConst, "edge_ucommon_const"},
      {ScType::EdgeDCommonConst, "edge_dcommon_const"},

      {ScType::EdgeAccess, "edge_access"},

      {ScType::EdgeAccessConstPosPerm, "edge_access_const_pos_perm"},
      {ScType::EdgeAccessConstNegPerm, "edge_access_const_neg_perm"},
      {ScType::EdgeAccessConstFuzPerm, "edge_access_const_fuz_perm"},
      {ScType::EdgeAccessConstPosTemp, "edge_access_const_pos_temp"},
      {ScType::EdgeAccessConstNegTemp, "edge_access_const_neg_temp"},
      {ScType::EdgeAccessConstFuzTemp, "edge_access_const_fuz_temp"},

      {ScType::EdgeUCommonVar, "edge_ucommon_var"},
      {ScType::EdgeDCommonVar, "edge_dcommon_var"},
      {ScType::EdgeAccessVarPosPerm, "edge_access_var_pos_perm"},
      {ScType::EdgeAccessVarNegPerm, "edge_access_var_neg_perm"},
      {ScType::EdgeAccessVarFuzPerm, "edge_access_var_fuz_perm"},
      {ScType::EdgeAccessVarPosTemp, "edge_access_var_pos_temp"},
      {ScType::EdgeAccessVarNegTemp, "edge_access_var_neg_temp"},
      {ScType::EdgeAccessVarFuzTemp, "edge_access_var_fuz_temp"}};

private:
  bool checkAction(ScAddr const & actionAddr);
  std::string get_str_ScType(ScAddr const & node);
  bool write_in_file(std::string&);
};

}  // namespace identifiersModule
