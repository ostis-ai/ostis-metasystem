/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "generate_response_constants.hpp"

namespace generateResponseModule
{
std::string const GenerateResponseConstants::generateAnswerAgentClassName = "GenerateAnswerAgent";
std::string const GenerateResponseConstants::messageAddrParamName = "messageAddr";
std::string const GenerateResponseConstants::answerAddrParamName = "answerAddr";
std::string const GenerateResponseConstants::roleRelationMappingTemplateName = "role_relation_mapping_search_template";
std::string const GenerateResponseConstants::targetRoleRelationVarName = "_target_role";
std::string const GenerateResponseConstants::paramVarName = "_param";
std::string const GenerateResponseConstants::messageVarName = "_message";
std::string const GenerateResponseConstants::messageClassVarName = "_message_class";
std::string const GenerateResponseConstants::actionClassVarName = "_action";

}  // namespace generateResponseModule
