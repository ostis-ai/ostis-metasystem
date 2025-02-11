/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "agent/check-duplicate-constructions-agent.hpp"
#include "check-synonym-objects-agent.hpp"
#include "check-synonym-relations-agent.hpp"

using namespace verificationModule;

SC_MODULE_REGISTER(VerificationModule)->Agent<CheckDuplicateConstructionsAgent>();
SC_MODULE_REGISTER(VerificationModule)->Agent<CheckSynonymObjectsAgent>();
SC_MODULE_REGISTER(VerificationModule)->Agent<CheckSynonymRelationsAgent>();
