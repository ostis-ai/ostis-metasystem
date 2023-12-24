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

    ScIterator3Ptr const & iterator3PtrEdgeBelongsToNrelSystemIdtf = ms_context->Iterator3(
            nrelSystemIdtf,
            ScType::EdgeAccessConstPosPerm,
            ScType::EdgeDCommonConst
            );

    ScAddr edgeBelongsToNrelSystemIdtf;
    ScAddr sourceOfEdgeBelongsToNrelSystemIdtf;
    ScAddr targetOfEdgeBelongsToNrelSystemIdtf;

    std::string systemIdtf;
    std::string mainIdtf;
    std::string strType;

    while (iterator3PtrEdgeBelongsToNrelSystemIdtf->Next()) {
        edgeBelongsToNrelSystemIdtf = iterator3PtrEdgeBelongsToNrelSystemIdtf->Get(2);
        sourceOfEdgeBelongsToNrelSystemIdtf = ms_context->GetEdgeSource(edgeBelongsToNrelSystemIdtf);
        targetOfEdgeBelongsToNrelSystemIdtf = ms_context->GetEdgeTarget(edgeBelongsToNrelSystemIdtf);

        bool isOneSystemIdtf = isOneEdgeFromNodeToLinkUnderRel(
                sourceOfEdgeBelongsToNrelSystemIdtf,
                nrelSystemIdtf);
        bool isOneMainRuIdtf = isOneEdgeFromNodeToLinkUnderRel(
                sourceOfEdgeBelongsToNrelSystemIdtf,
                scAgentsCommon::CoreKeynodes::nrel_main_idtf);

        //try{
            if (isOneSystemIdtf && isOneMainRuIdtf) {
                ms_context->GetLinkContent(targetOfEdgeBelongsToNrelSystemIdtf, systemIdtf);
                mainIdtf = utils::CommonUtils::getMainIdtf(
                        &m_memoryCtx,
                        sourceOfEdgeBelongsToNrelSystemIdtf,
                        {scAgentsCommon::CoreKeynodes::lang_ru});
                strType = getStrScType(sourceOfEdgeBelongsToNrelSystemIdtf);

                if (!mainIdtf.empty() && !strType.empty()) {
                    streamIdtfs << "{\"" << mainIdtf << "\", "
                                << "{\"" << systemIdtf << "\", \""
                                << strType << "\"} },\n";
                }
            }
//            else {
//                ms_context->GetLinkContent(targetOfEdgeBelongsToNrelSystemIdtf, systemIdtf);
//                SC_THROW_EXCEPTION(utils::ScException,
//                                   "You have mistakes in the formalisation of " + systemIdtf);
//            }
//        }
//        catch (utils::ScException & exception)
//        {
//            SC_LOG_ERROR(exception.Description());
//            return SC_RESULT_ERROR;
//        }
        }

        string strIdtfs(streamIdtfs.str());
        // Remove last symbols "," and "\n"
        if(!strIdtfs.empty()) {
            strIdtfs.pop_back();
            if(!strIdtfs.empty()) {
                strIdtfs.pop_back();
            }
        }

        bool const & resultOfWrite = writeInFile(strIdtfs);

        if (resultOfWrite) {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,
                                   scAgentsCommon::CoreKeynodes::question_finished_successfully,
                                   actionAddr);
            SC_LOG_DEBUG("File has been created");
        } else {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,
                                   scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully,
                                   actionAddr);
            SC_LOG_ERROR("File hasn't been created");
        }
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,
                               scAgentsCommon::CoreKeynodes::question_finished,
                               actionAddr);
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

bool TranslateMainSystemIdtfsFromScToFileAgent::isOneEdgeFromNodeToLinkUnderRel(ScAddr const & node, ScAddr const & nodeRelation) {
    ScIterator5Ptr const & iterator5PtrCheckOnlyOneIdtf = ms_context->Iterator5(
            node,
            ScType::EdgeDCommonConst,
            ScType::Link,
            ScType::EdgeAccessConstPosPerm,
            nodeRelation
    );

    if(nodeRelation == CoreKeynodes::nrel_main_idtf){
        bool const numberOfRuLinks = countLinkWithLang(iterator5PtrCheckOnlyOneIdtf, scAgentsCommon::CoreKeynodes::lang_ru);
        return numberOfRuLinks;
    }

    int times = 0;
    while(iterator5PtrCheckOnlyOneIdtf->Next()){
        times++;
    }
    if (times == ONE) {
        return true;
    }
    return false;
}

int TranslateMainSystemIdtfsFromScToFileAgent::countLinkWithLang(ScIterator5Ptr const & iterator5PtrCheckOnlyOneIdtf, ScAddr const & lang) {
    int times = 0;
    while(iterator5PtrCheckOnlyOneIdtf->Next()){
        ScAddr link = iterator5PtrCheckOnlyOneIdtf->Get(2);
        bool is_lang = ms_context->HelperCheckEdge(lang, link, ScType::EdgeAccessConstPosPerm);
        if (is_lang) {
            times++;
        }
    }
    if (times == ONE) {
        return true;
    }
    return false;
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

bool TranslateMainSystemIdtfsFromScToFileAgent::writeInFile(std::string const & strIdentifiers) {
    try {
        std::ofstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
        if (file.is_open()) {
            file << strIdentifiers;
        }
        file.close();
    }
    catch (std::exception const & err) {
        SC_LOG_ERROR(err.what());
        return false;
    }
    return true;
}
