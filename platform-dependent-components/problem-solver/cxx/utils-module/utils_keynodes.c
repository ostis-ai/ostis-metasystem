/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.h"
#include "utils_keynodes.h"
#include "sc-common/sc_keynodes.h"
//#include <glib.h>

//sc_addr keynode_question_erase_elements;
sc_addr keynode_question_compute_setpower;
sc_addr keynode_question_compute_set_symmetric_difference;
sc_addr keynode_init_memory_generated_structure;

sc_addr keynode_question_initiated;
sc_addr keynode_question_finished;
sc_addr keynode_question_finished_successfully;
sc_addr keynode_question_finished_unsuccessfully;

sc_addr keynode_nrel_answer;

sc_addr keynode_set_symmetric_difference;
//sc_addr keynode_set_power;
sc_addr keynode_set_power;

sc_addr keynode_rrel_1;
sc_addr keynode_rrel_2;

sc_addr keynode_set;
sc_addr keynode_exact_represented;

sc_addr keynode_system_element;


//const char keynode_question_erase_elements_str[] = "question_erase_elements";
const char keynode_question_compute_setpower_str[] = "question_compute_setpower";
const char keynode_question_compute_set_symmetric_difference_str[] = "question_compute_set_symmetric_difference";

const char keynode_question_initiated_str[] = "question_initiated";
const char keynode_question_finished_str[] = "question_finished";
const char keynode_question_finished_successfully_str[] = "question_finished_successfully";
const char keynode_question_finished_unsuccessfully_str[] = "question_finished_unsuccessfully";

const char keynode_nrel_answer_str[] = "nrel_answer";

const char keynode_set_symmetric_difference_str[] = "set_symmetric_difference";
//const char keynode_set_power_str[] = "set_power";
const char keynode_set_power_str[] = "set_power";

const char keynode_rrel_1_str[] = "rrel_1";
const char keynode_rrel_2_str[] = "rrel_2";

const char keynode_set_str[] = "set";
const char keynode_exact_represented_str[] = "represented_exactly";

const char keynode_system_element_str[] = "system_element";

sc_result utils_keynodes_initialize(sc_addr const init_memory_generated_structure)
{
//  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_erase_elements, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_compute_setpower, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_compute_set_symmetric_difference, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_initiated, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_finished, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_finished_successfully, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_question_finished_unsuccessfully, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_nrel_answer, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_set_symmetric_difference, init_memory_generated_structure);
//  RESOLVE_KEYNODE(s_utils_ctx, keynode_set_power, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_set_power, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_rrel_1, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_rrel_2, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_system_element, init_memory_generated_structure);

  RESOLVE_KEYNODE(s_utils_ctx, keynode_set, init_memory_generated_structure);
  RESOLVE_KEYNODE(s_utils_ctx, keynode_exact_represented, init_memory_generated_structure);

  keynode_init_memory_generated_structure = init_memory_generated_structure;

  return SC_RESULT_OK;
}
