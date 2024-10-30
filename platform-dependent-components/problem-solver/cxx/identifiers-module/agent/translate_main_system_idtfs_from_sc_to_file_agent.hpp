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
      {ScType::NodeLink, "sc_link"},
      {ScType::NodeLinkClass, "sc_link"},
      {ScType::Unknown, "sc_node"},

      {ScType::ConstNode, "sc_node"},
      {ScType::VarNode, "sc_node"},

      {ScType::ConstNodeLink, "sc_link"},
      {ScType::VarNodeLink, "sc_link"},

      {ScType::NodeStructure, "sc_node_struct"},
      {ScType::NodeTuple, "sc_node_tuple"},
      {ScType::NodeRole, "sc_node_role_relation"},
      {ScType::NodeNoRole, "sc_node_norole_relation"},
      {ScType::NodeClass, "sc_node_class"},
      {ScType::NodeMaterial, "sc_node_material"},

      {ScType::ConstNodeStructure, "sc_node_struct"},
      {ScType::ConstNodeTuple, "sc_node_tuple"},
      {ScType::ConstNodeRole, "sc_node_role_relation"},
      {ScType::ConstNodeNoRole, "sc_node_norole_relation"},
      {ScType::ConstNodeClass, "sc_node_class"},
      {ScType::ConstNodeMaterial, "sc_node_material"},

      {ScType::VarNodeStructure, "sc_node_struct"},
      {ScType::VarNodeTuple, "sc_node_tuple"},
      {ScType::ScType::VarNodeRole, "sc_node_role_relation"},
      {ScType::VarNodeNoRole, "sc_node_norole_relation"},
      {ScType::VarNodeClass, "sc_node_class"},
      {ScType::VarNodeMaterial, "sc_node_material"}};

  std::map<ScType, std::string> ScTypesOfEdgesWithSCsClasses = {
      {ScType::CommonEdge, "sc_edge"},
      {ScType::VarCommonArc, "sc_edge_common"},

      {ScType::ConstCommonEdge, "<=>"},
      {ScType::ConstCommonArc, "sc_edge_common"},

      {ScType::ConstTempPosArc, "sc_edge_access"},

      {ScType::ConstPermPosArc, "sc_edge_main"},
      {ScType::ConstPermNegArc, "sc_edge_main"},
      {ScType::ConstFuzArc, "sc_edge_main"},
      {ScType::ConstTempPosArc, "sc_edge_main"},
      {ScType::ConstTempNegArc, "sc_edge_main"},
      {ScType::ConstFuzArc, "sc_edge_main"},

      {ScType::VarCommonEdge, "_<=>"},
      {ScType::VarCommonArc, "sc_edge_common"},
      {ScType::VarPosArc, "sc_edge_main"},
      {ScType::VarPermNegArc, "sc_edge_main"},
      {ScType::VarFuzArc, "sc_edge_main"},
      {ScType::VarTempPosArc, "sc_edge_main"},
      {ScType::VarTempNegArc, "sc_edge_main"},
      {ScType::VarFuzArc, "sc_edge_main"}};

private:
  
  std::string GetStrScType(ScAddr const & node);
  static std::string GetSystemIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static std::string GetMainIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static bool WriteInFile(std::string const & strIdtfs);
};

}  // namespace identifiersModule
