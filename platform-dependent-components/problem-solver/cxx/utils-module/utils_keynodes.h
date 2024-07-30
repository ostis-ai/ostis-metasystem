/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _utils_keynodes_h_
#define _utils_keynodes_h_

#include "sc-core/sc_memory.h"

//extern sc_addr keynode_question_erase_elements;
extern sc_addr keynode_question_compute_setpower;
extern sc_addr keynode_question_compute_set_symmetric_difference;
extern sc_addr keynode_init_memory_generated_structure;

extern sc_addr keynode_question_initiated;
extern sc_addr keynode_question_finished;
extern sc_addr keynode_question_finished_successfully;
extern sc_addr keynode_question_finished_unsuccessfully;

extern sc_addr keynode_nrel_answer;

extern sc_addr keynode_set_symmetric_difference;
//extern sc_addr keynode_set_power;
extern sc_addr keynode_set_power;

extern sc_addr keynode_rrel_1;
extern sc_addr keynode_rrel_2;

extern sc_addr keynode_set;
extern sc_addr keynode_exact_represented;

extern sc_addr keynode_system_element;

sc_result utils_keynodes_initialize(sc_addr const init_memory_generated_structure);

#endif
