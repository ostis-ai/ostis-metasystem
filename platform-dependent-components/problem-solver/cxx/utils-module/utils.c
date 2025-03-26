/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.h"
#include "utils_keynodes.h"
#include "agents/compute_setpower.h"
#include "agents/compute_set_symmetric_difference.h"
//#include "../sc-search/search_keynodes.h"
//#include "sc-core/sc_memory_context_manager.h"
#include "sc-core/sc-store/sc_event.h"

sc_memory_context * s_utils_ctx = 0;

sc_event * event_compute_setpower;
sc_event * event_compute_set_symmetric_difference;



_SC_EXT_EXTERN sc_result
sc_module_initialize_with_init_memory_generated_structure(sc_addr const init_memory_generated_structure)
{
//  printf("utils initialization: enter\n");
//  s_utils_ctx = sc_memory_context_new(sc_access_lvl_make_max);
//  s_utils_ctx = s_memory_default_ctx;
  s_utils_ctx = sc_memory_context_new_ext(SC_ADDR_EMPTY);

  if (utils_keynodes_initialize(init_memory_generated_structure) != SC_RESULT_OK)
    return SC_RESULT_ERROR;

  event_compute_setpower = sc_event_new(
      s_utils_ctx,
      keynode_question_initiated,
      SC_EVENT_ADD_OUTPUT_ARC,
      null_ptr,
      agent_compute_setpower,
      null_ptr);
  if (event_compute_setpower == null_ptr)
    return SC_RESULT_ERROR;

  event_compute_set_symmetric_difference = sc_event_new(
      s_utils_ctx,
      keynode_question_initiated,
      SC_EVENT_ADD_OUTPUT_ARC,
      null_ptr,
      agent_compute_set_symmetric_difference,
      null_ptr);
  if (event_compute_set_symmetric_difference == null_ptr)
    return SC_RESULT_ERROR;

//  printf("utils initialization: leave");
  return SC_RESULT_OK;
}

_SC_EXT_EXTERN sc_uint32 sc_module_load_priority()
{
  return 1000;
}

_SC_EXT_EXTERN sc_result sc_module_shutdown()
{
  sc_result res = SC_RESULT_OK;

  if (event_compute_setpower)
    sc_event_destroy(event_compute_setpower);

  if (event_compute_set_symmetric_difference)
    sc_event_destroy(event_compute_set_symmetric_difference);


  sc_memory_context_free(s_utils_ctx);

  return res;
}
