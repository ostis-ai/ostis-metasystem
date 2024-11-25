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

  ScResult DoProgram(ScAction & action) override;

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

      {ScType::NodeStructure, "sc_node_structure"},
      {ScType::NodeTuple, "sc_node_tuple"},
      {ScType::NodeRole, "sc_node_role_relation"},
      {ScType::NodeNonRole, "sc_node_non_role_relation"},
      {ScType::NodeClass, "sc_node_class"},
      {ScType::NodeSuperclass, "sc_node_superclass"},
      {ScType::NodeMaterial, "sc_node_material"},

      {ScType::ConstNodeStructure, "sc_node_structure"},
      {ScType::ConstNodeTuple, "sc_node_tuple"},
      {ScType::ConstNodeRole, "sc_node_role_relation"},
      {ScType::ConstNodeNonRole, "sc_node_non_role_relation"},
      {ScType::ConstNodeClass, "sc_node_class"},
      {ScType::ConstNodeMaterial, "sc_node_material"},

      {ScType::VarNodeStructure, "sc_node_structure"},
      {ScType::VarNodeTuple, "sc_node_tuple"},
      {ScType::VarNodeRole, "sc_node_role_relation"},
      {ScType::VarNodeNonRole, "sc_node_non_role_relation"},
      {ScType::VarNodeClass, "sc_node_class"},
      {ScType::VarNodeMaterial, "sc_node_material"}};

  std::map<ScType, std::string> ScTypesOfEdgesWithSCsClasses = {
      {ScType::CommonEdge, "sc_common_edge"},
      {ScType::CommonArc, "sc_common_arc"},
      {ScType::MembershipArc, "sc_membership_arc"},

      {ScType::ConstCommonEdge, "sc_common_edge"},
      {ScType::ConstCommonArc, "sc_common_arc"},

      {ScType::ConstPermPosArc, "sc_main_arc"},
      {ScType::ConstPermNegArc, "sc_main_arc"},
      {ScType::ConstTempPosArc, "sc_main_arc"},
      {ScType::ConstTempNegArc, "sc_main_arc"},

      {ScType::VarCommonEdge, "sc_common_edge"},
      {ScType::VarCommonArc, "sc_main_arc"},
      {ScType::VarPosArc, "sc_main_arc"},
      {ScType::VarFuzArc, "sc_main_arc"},
      {ScType::VarPermNegArc, "sc_main_arc"},
      {ScType::VarTempPosArc, "sc_main_arc"},
      {ScType::VarTempNegArc, "sc_main_arc"}};
      
private:
  
  std::string GetStrScType(ScAddr const & node);
  static std::string GetSystemIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static std::string GetMainIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node);
  static bool WriteInFile(std::string const & strIdtfs);
};

}  // namespace identifiersModule
