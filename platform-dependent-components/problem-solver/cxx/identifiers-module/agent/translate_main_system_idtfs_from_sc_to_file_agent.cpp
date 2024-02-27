/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "keynodes/identifiers_keynodes.hpp"
#include "translate_main_system_idtfs_from_sc_to_file_agent.hpp"

using namespace identifiersModule;

SC_AGENT_IMPLEMENTATION(TranslateMainSystemIdtfsFromScToFileAgent)
{
    ScAddr const & actionAddr = otherAddr;

    if (!CheckAction(actionAddr)) {
        return SC_RESULT_OK;
    }

    SC_LOG_DEBUG("TranslateMainSystemIdtfsFromScToFileAgent started");

    // TODO: replace by CoreKeynodes::nrel_system_identifier after release
    ScAddr const & nrelSystemIdtf = m_memoryCtx.HelperFindBySystemIdtf("nrel_system_identifier");

    std::stringstream streamIdtfs;

    ScIterator3Ptr const & iterator3PtrEdgeBelongsToNrelSystemIdtf = m_memoryCtx.Iterator3(
            nrelSystemIdtf,
            ScType::EdgeAccessConstPosPerm,
            ScType::EdgeDCommonConst
            );

    ScAddr edgeBelongsToNrelSystemIdtf;
    ScAddr sourceOfEdgeBelongsToNrelSystemIdtf;

    std::string systemIdentifier;
    std::string mainIdentifier;
    std::string stringType;

    while (iterator3PtrEdgeBelongsToNrelSystemIdtf->Next()) {
        edgeBelongsToNrelSystemIdtf = iterator3PtrEdgeBelongsToNrelSystemIdtf->Get(2);
        sourceOfEdgeBelongsToNrelSystemIdtf = m_memoryCtx.GetEdgeSource(edgeBelongsToNrelSystemIdtf);
        try {
            systemIdentifier = GetSystemIdtfAndVerifyNode(m_memoryCtx, sourceOfEdgeBelongsToNrelSystemIdtf);
            mainIdentifier = GetMainIdtfAndVerifyNode(m_memoryCtx, sourceOfEdgeBelongsToNrelSystemIdtf);
            stringType = GetStrScType(sourceOfEdgeBelongsToNrelSystemIdtf);

            if (!systemIdentifier.empty() && !mainIdentifier.empty() && !stringType.empty()) {
                streamIdtfs << "{\"" << mainIdentifier << "\", "
                            << "{\"" << systemIdentifier << "\", \""
                            << stringType << "\"} },\n";
            }
        }
        catch (utils::ScException const & exception) {
            SC_LOG_ERROR(exception.Description());
            utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
            return SC_RESULT_ERROR;
        }
    }

        std::string strIdtfs(streamIdtfs.str());
        // Remove last symbols "," and "\n"
        if(!strIdtfs.empty()) {
            strIdtfs.pop_back();
            if(!strIdtfs.empty()) {
                strIdtfs.pop_back();
            }
        }

        bool const & resultOfWrite = WriteInFile(strIdtfs);

        if (resultOfWrite) {
            SC_LOG_DEBUG("File has been created");
        } else {
            SC_LOG_ERROR("File hasn't been created");
        }
        utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, resultOfWrite);
        SC_LOG_DEBUG("TranslateMainSystemIdtfsFromScToFileAgent finished");
        return SC_RESULT_OK;
}

bool TranslateMainSystemIdtfsFromScToFileAgent::CheckAction(ScAddr const & actionAddr) {
    return m_memoryCtx.HelperCheckEdge(
            IdentifiersKeynodes::action_find_identifiers,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetSystemIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node) {
    std::string identifier;
    ScAddr identifierLink;
    ScIterator5Ptr const & iterator5PtrCheckOnlyOneIdtf = m_memoryCtx.Iterator5(
            node,
            ScType::EdgeDCommonConst,
            ScType::LinkConst,
            ScType::EdgeAccessConstPosPerm,
            m_memoryCtx.HelperFindBySystemIdtf("nrel_system_identifier")
    );

    if(iterator5PtrCheckOnlyOneIdtf->Next()) {
        identifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
        if (iterator5PtrCheckOnlyOneIdtf->Next()) {
            SC_THROW_EXCEPTION(utils::ScException, "You have more than one system identifier for " + identifier);
        }
        m_memoryCtx.GetLinkContent(identifierLink, identifier);
    }
    return identifier;
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetMainIdtfAndVerifyNode(ScMemoryContext & m_memoryCtx, ScAddr const & node) {
    std::string identifier;
    ScAddr mainIdentifierLink;
    ScAddr mainAnotherIdentifierLink;
    ScIterator5Ptr const &iterator5PtrCheckOnlyOneIdtf = m_memoryCtx.Iterator5(
            node,
            ScType::EdgeDCommonConst,
            ScType::LinkConst,
            ScType::EdgeAccessConstPosPerm,
            scAgentsCommon::CoreKeynodes::nrel_main_idtf
    );

    bool isLangRu;
    while (iterator5PtrCheckOnlyOneIdtf->Next()) {
        mainIdentifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
        isLangRu = m_memoryCtx.HelperCheckEdge(scAgentsCommon::CoreKeynodes::lang_ru,
                                               mainIdentifierLink,
                                               ScType::EdgeAccessConstPosPerm);
        if (isLangRu) {
            while (iterator5PtrCheckOnlyOneIdtf->Next()) {
                mainAnotherIdentifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
                isLangRu = m_memoryCtx.HelperCheckEdge(scAgentsCommon::CoreKeynodes::lang_ru,
                                                       mainAnotherIdentifierLink,
                                                       ScType::EdgeAccessConstPosPerm);
                if (isLangRu) {
                    return identifier;
                }
            }
            m_memoryCtx.GetLinkContent(mainIdentifierLink, identifier);
            break;
        }
    }
    return identifier;
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetStrScType(ScAddr const & node) {
    std::string strType;
    ScType const & type = m_memoryCtx.GetElementType(node);
    if (ScTypesOfNodesWithSCsClasses.count(type)) {
        strType = ScTypesOfNodesWithSCsClasses[type];
    } else if (ScTypesOfEdgesWithSCsClasses.count(type)) {
        strType = ScTypesOfEdgesWithSCsClasses[type];
    }
    return strType;
}

bool TranslateMainSystemIdtfsFromScToFileAgent::WriteInFile(std::string const & strIdentifiers) {
    try {
        std::ofstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
        if (file.is_open()) {
            file << strIdentifiers;
        }
        file.close();
    }
    catch (std::exception const & exception) {
        SC_LOG_ERROR(exception.what());
        return false;
    }
    return true;
}
