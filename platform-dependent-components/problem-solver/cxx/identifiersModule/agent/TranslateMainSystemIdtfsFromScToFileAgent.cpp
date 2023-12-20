/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/IdentifiersKeynodes.hpp"
#include "TranslateMainSystemIdtfsFromScToFileAgent.hpp"

using namespace identifiersModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(TranslateMainSystemIdtfsFromScToFileAgent)
{
    ScAddr const & actionAddr = otherAddr;

    if (!checkAction(actionAddr)) {
        return SC_RESULT_OK;
    }

    SC_LOG_DEBUG("TranslateMainSystemIdtfsFromScToFileAgent started");

    // TODO: replace by CoreKeynodes::nrel_system_identifier after release
    ScAddr const & nrelSystemIdtf = ms_context->HelperFindBySystemIdtf("nrel_system_identifier");

    std::stringstream streamIdtfs;

    ScIterator3Ptr Iterator3PtrEdgeBelongToNrelSystemIdtf = ms_context->Iterator3(
            nrelSystemIdtf,
            ScType::EdgeAccessConstPosPerm,
            ScType::EdgeDCommonConst
            );

    ScAddr edgeNrelSystemIdtf;
    ScAddr node;
    ScAddr nodeSystemIdtf;

    std::string systemIdtf;
    std::string mainIdtf;
    std::string strType;

    while (Iterator3PtrEdgeBelongToNrelSystemIdtf->Next()) {
        edgeNrelSystemIdtf = Iterator3PtrEdgeBelongToNrelSystemIdtf->Get(2);
        node = ms_context->GetEdgeSource(edgeNrelSystemIdtf);
        nodeSystemIdtf = ms_context->GetEdgeTarget(edgeNrelSystemIdtf);

        ms_context->GetLinkContent(nodeSystemIdtf, systemIdtf);
        mainIdtf = utils::CommonUtils::getMainIdtf(&m_memoryCtx, node,{scAgentsCommon::CoreKeynodes::lang_ru});
        strType = getStrScType(node);

        if (!mainIdtf.empty() && !strType.empty()) {
            streamIdtfs << "{\"" << mainIdtf << "\", "
                        << "{\"" << systemIdtf << "\", \""
                        << strType << "\"} },\n";
        }
    }

    string strIdtfs(streamIdtfs.str());
    // Remove last symbols "," and "\n"
    strIdtfs.pop_back();
    strIdtfs.pop_back();

    bool const & resultOfWrite = writeInFile(strIdtfs);

    if (resultOfWrite) {
        SC_LOG_DEBUG("File has been created");
    } else {
        SC_LOG_ERROR("File hasn't been created");
    }

    SC_LOG_DEBUG("TranslateMainSystemIdtfsFromScToFileAgent finished");
    return SC_RESULT_OK;
}

bool TranslateMainSystemIdtfsFromScToFileAgent::checkAction(ScAddr const & actionAddr) {
    return m_memoryCtx.HelperCheckEdge(
            IdentifiersKeynodes::action_find_identifiers,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::getStrScType(ScAddr const & node) {
    std::string strType;
    ScType const & type = ms_context->GetElementType(node);
    if (ScTypesOfNodesWithSCsClasses.count(type)) {
        strType = ScTypesOfNodesWithSCsClasses[type];
    } else if (ScTypesOfEdgesWithSCsClasses.count(type)) {
        strType = ScTypesOfEdgesWithSCsClasses[type];
    }
    return strType;
}

bool TranslateMainSystemIdtfsFromScToFileAgent::writeInFile(std::string const & strIdtfs) {
    try {
        std::ofstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
        if (file.is_open()) {
            file << strIdtfs;
        }
        file.close();
    }
    catch (std::exception const & err) {
        SC_LOG_ERROR(err.what());
        return false;
    }
    return true;
}
