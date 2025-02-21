/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "agent/check-duplicate-constructions-agent.hpp"

#include "agent/not-enough-specified-objects-search-agent.hpp"
#include "agent/not-enough-specified-objects-search-module/not-enough-specified-relations-search-agent.hpp"
#include "agent/not-enough-specified-objects-search-module/not-enough-specified-structures-search-agent.hpp"
#include "agent/not-enough-specified-objects-search-module/not-enough-specified-classes-search-agent.hpp"
using namespace verificationModule;

SC_MODULE_REGISTER(VerificationModule)
    ->Agent<CheckDuplicateConstructionsAgent>()
    ->Agent<SearchNotEnoughSpecifiedObjectsAgent>();
