/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "identifiers_module.hpp"

#include <sc-memory/sc_memory.hpp>

#include "agent/translate_main_system_idtfs_from_sc_to_file_agent.hpp"

using namespace identifiersModule;

SC_MODULE_REGISTER(IdentifiersModule)->Agent<TranslateMainSystemIdtfsFromScToFileAgent>();
