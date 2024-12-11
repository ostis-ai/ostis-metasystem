/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "subject_domain_module.hpp"

#include "agent/get_decomposition_agent.hpp"
#include "agent/add_section_agent.hpp"
#include "agent/remove_section_agent.hpp"

using namespace sectionsModule;

SC_MODULE_REGISTER(SectionsModule)
    ->Agent<GetDecompositionAgent>()
    ->Agent<AddSectionAgent>()
    ->Agent<RemoveSectionAgent>();
