/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "../utils.h"
#include "compute_setpower.h"
#include "../utils_keynodes.h"
#include "../utils_utils.h"
#include "../utils_defines.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
#include <stdbool.h>
#include <string.h>

#define sc_type_arc_pos_const_temp (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos|sc_type_arc_temp)
#define sc_type_arc_pos_const_actual (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos)
#define sc_type_arc_pos_const (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos)

sc_result agent_compute_setpower(const sc_event * event, sc_addr arg)
{
  sc_addr question, answer;
  sc_iterator3 *it1;
  sc_bool sys_off = SC_TRUE;

  printf("agent compute setpower: enter\n");

  if (!sc_memory_get_arc_end(s_utils_ctx, arg, &question))
    return SC_RESULT_ERROR_INVALID_PARAMS;

  // check question type
  if (sc_helper_check_arc(
          s_utils_ctx, keynode_question_compute_setpower, question, sc_type_arc_pos_const_perm) == SC_FALSE)
    return SC_RESULT_ERROR_INVALID_TYPE;

  printf("agent compute setpower: started\n");

  answer = create_answer_node();
  sc_uint32 power = 0;
  // get operation argument
  printf("agent compute setpower: iterating...\n");
  it1 = sc_iterator3_f_a_a_new(s_utils_ctx, question, sc_type_arc_pos_const_perm, 0);

  printf("agent compute setpower: checking...\n");
  if (sc_iterator3_next(it1) == SC_TRUE) {
    if (sc_helper_check_arc(
          s_utils_ctx, keynode_set, sc_iterator3_value(it1,2), sc_type_arc_pos_const_actual) == SC_FALSE) {
      printf("agent compute setpower: not set\n");
      return SC_RESULT_ERROR_INVALID_TYPE;
    }

    printf("agent compute setpower: found element\n");

    sc_addr set = sc_iterator3_value(it1,2);

    sc_iterator3_free(it1);

    if (sc_helper_check_arc(
          s_utils_ctx, keynode_exact_represented, set, sc_type_arc_pos_const_actual) == SC_TRUE) {

	it1 = sc_iterator3_f_a_a_new(s_utils_ctx, set, sc_type_arc_pos_const_perm, 0);
	while ((sc_iterator3_next(it1) == SC_TRUE) && (power < 0xffffffff)) ++power;
        sc_iterator3_free(it1);
    }
    else {
	it1 = sc_iterator3_f_a_a_new(s_utils_ctx, set, sc_type_arc_pos_const_perm, 0);
	if (sc_iterator3_next(it1) == SC_TRUE) ++power;
        sc_iterator3_free(it1);
    }

    printf("agent compute setpower: link generating...\n");
    // set content
    sc_addr link = sc_memory_link_new(s_utils_ctx);
    sc_stream * stream = sc_stream_memory_new((sc_char const *)&power,sizeof(power),SC_STREAM_FLAG_READ,SC_FALSE);
    if (sc_memory_set_link_content(s_utils_ctx, link, stream) != SC_RESULT_OK) {
        sc_stream_free(stream);
	return SC_RESULT_ERROR;
    }

    appendIntoAnswer(answer, link);

    sc_stream_free(stream);

    printf("agent compute setpower: answer forming...\n");
    // connect set power
    if (power < 0xffffffff) {

       sc_char s[10];
       snprintf(s,9,"%u",power);

       printf("agent compute setpower: trying to find element...\n");
       // lock memory??
       sc_addr node;
       if (sc_helper_find_element_by_system_identifier(s_utils_ctx,s,strlen(s),&node)==SC_RESULT_OK);
       else {
         node = sc_memory_node_new(s_utils_ctx,sc_type_const);
         sc_helper_set_system_identifier(s_utils_ctx,node,s,strlen(s));
       }
       // unlock memory??

       printf("agent compute setpower: continue...\n");
       appendIntoAnswer(answer, node);
       sc_addr arc = updateArc(s_utils_ctx,keynode_set_power,node,sc_type_arc_pos_const_perm);
       appendIntoAnswer(answer, arc);
       arc = updateArc(s_utils_ctx,node,set,sc_type_arc_pos_const_perm);
       appendIntoAnswer(answer, arc);
//       appendIntoAnswer(answer, keynode_set_power);
       appendIntoAnswer(answer, keynode_set_power);
    }
    else {
    }
  }
  
  connect_answer_to_question(question, answer);
  finish_question(question);

  printf("agent compute setpower: leave\n");

  return SC_RESULT_OK;
}
