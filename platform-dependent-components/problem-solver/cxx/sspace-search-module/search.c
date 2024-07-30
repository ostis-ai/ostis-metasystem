/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search.h"
#include "search_agents.h"
#include "search_keynodes.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
//#include "sc-core/sc_memory_context_manager.h"

sc_memory_context * s_default_ctx = 0;

sc_event * event_question_search_extensional_closure;
sc_event * event_question_search_fullconnected_closure;

// --------------------- Module ------------------------

sc_result sc_module_initialize_with_init_memory_generated_structure(sc_addr const init_memory_generated_structure)
{
//  s_default_ctx = sc_memory_context_new(sc_access_lvl_make_min);
//  s_default_ctx = s_memory_default_ctx;
  s_default_ctx = sc_memory_context_new_ext(SC_ADDR_EMPTY);

  if (search_keynodes_initialize(s_default_ctx, init_memory_generated_structure) != SC_RESULT_OK)
    return SC_RESULT_ERROR;


  event_question_search_extensional_closure = sc_event_new(
      s_default_ctx, keynode_question_initiated, SC_EVENT_ADD_OUTPUT_ARC, null_ptr, agent_search_extensional_closure, null_ptr);
  if (event_question_search_extensional_closure == null_ptr)
    return SC_RESULT_ERROR;

  event_question_search_fullconnected_closure = sc_event_new(
      s_default_ctx, keynode_question_initiated, SC_EVENT_ADD_OUTPUT_ARC, null_ptr, agent_search_fullconnected_closure, null_ptr);
  if (event_question_search_fullconnected_closure == null_ptr)
    return SC_RESULT_ERROR;


  return SC_RESULT_OK;
}

_SC_EXT_EXTERN sc_uint32 sc_module_load_priority()
{
  return 0;
}

sc_result sc_module_shutdown()
{
  if (event_question_search_extensional_closure)
    sc_event_destroy(event_question_search_extensional_closure);
  if (event_question_search_fullconnected_closure)
    sc_event_destroy(event_question_search_fullconnected_closure);

  sc_memory_context_free(s_default_ctx);

  return SC_RESULT_OK;
}
