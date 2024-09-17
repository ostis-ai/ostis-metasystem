/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace identifiersModule
{
class TranslateMainSystemIdtfsFromScToFileAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  std::map<ScType, std::string> ScTypesOfNodesWithSCsClasses = {
      {ScType::Const, "sc_node"},
      {ScType::Var, "sc_node"},

      {ScType::Node, "sc_node"},
      {ScType::Link, "sc_link"},
      {ScType::LinkClass, "sc_link"},
      {ScType::Unknown, "sc_node"},

      {ScType::NodeConst, "sc_node"},
      {ScType::NodeVar, "sc_node"},

      {ScType::LinkConst, "sc_link"},
      {ScType::LinkVar, "sc_link"},

      {ScType::NodeStruct, "sc_node_struct"},
      {ScType::NodeTuple, "sc_node_tuple"},
      {ScType::NodeRole, "sc_node_role_relation"},
      {ScType::NodeNoRole, "sc_node_norole_relation"},
      {ScType::NodeClass, "sc_node_class"},
      {ScType::NodeAbstract, "sc_node"},
      {ScType::NodeMaterial, "sc_node_material"},

      {ScType::NodeConstStruct, "sc_node_struct"},
      {ScType::NodeConstTuple, "sc_node_tuple"},
      {ScType::NodeConstRole, "sc_node_role_relation"},
      {ScType::NodeConstNoRole, "sc_node_norole_relation"},
      {ScType::NodeConstClass, "sc_node_class"},
      {ScType::NodeConstAbstract, "sc_node"},
      {ScType::NodeConstMaterial, "sc_node_material"},

      {ScType::NodeVarStruct, "sc_node_struct"},
      {ScType::NodeVarTuple, "sc_node_tuple"},
      {ScType::NodeVarRole, "sc_node_role_relation"},
      {ScType::NodeVarNoRole, "sc_node_norole_relation"},
      {ScType::NodeVarClass, "sc_node_class"},
      {ScType::NodeVarAbstract, "sc_node"},
      {ScType::NodeVarMaterial, "sc_node_material"}};

  std::map<ScType, std::string> ScTypesOfEdgesWithSCsClasses = {
      {ScType::EdgeUCommon, "sc_edge"},
      {ScType::EdgeDCommon, "sc_edge_common"},

      {ScType::EdgeUCommonConst, "<=>"},
      {ScType::EdgeDCommonConst, "sc_edge_common"},

      {ScType::EdgeAccess, "sc_edge_access"},

      {ScType::EdgeAccessConstPosPerm, "sc_edge_main"},
      {ScType::EdgeAccessConstNegPerm, "sc_edge_main"},
      {ScType::EdgeAccessConstFuzPerm, "sc_edge_main"},
      {ScType::EdgeAccessConstPosTemp, "sc_edge_main"},
      {ScType::EdgeAccessConstNegTemp, "sc_edge_main"},
      {ScType::EdgeAccessConstFuzTemp, "sc_edge_main"},

      {ScType::EdgeUCommonVar, "_<=>"},
      {ScType::EdgeDCommonVar, "sc_edge_common"},
      {ScType::EdgeAccessVarPosPerm, "sc_edge_main"},
      {ScType::EdgeAccessVarNegPerm, "sc_edge_main"},
      {ScType::EdgeAccessVarFuzPerm, "sc_edge_main"},
      {ScType::EdgeAccessVarPosTemp, "sc_edge_main"},
      {ScType::EdgeAccessVarNegTemp, "sc_edge_main"},
      {ScType::EdgeAccessVarFuzTemp, "sc_edge_main"}};

private:
  bool CheckAction(ScAddr const & actionAddr);
  std::string GetStrScType(ScAddr const & node);
  static std::string GetSystemIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static std::string GetMainIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static bool WriteInFile(std::string const & strIdtfs);
};

}  // namespace identifiersModule
