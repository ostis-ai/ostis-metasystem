/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "../utils.h"
#include "compute_set_symmetric_difference.h"
#include "../utils_keynodes.h"
#include "../utils_utils.h"
#include "../utils_defines.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
#include <stdbool.h>

#define sc_type_arc_neg_const_perm (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_neg|sc_type_arc_perm)
#define sc_type_arc_pos_const_temp (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos|sc_type_arc_temp)
#define sc_type_arc_pos_const_actual (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos)
#define sc_type_arc_pos_const (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos)
#define sc_type_arc_fuz_const (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_fuz)
#define sc_type_arc_const (sc_type)(sc_type_const|sc_type_arc_access)

sc_result agent_compute_set_symmetric_difference(const sc_event * event, sc_addr arg)
{
  sc_addr question, answer;
  sc_iterator3 *it1, *it2;
  sc_iterator5 *it51, *it52;
  sc_bool sys_off = SC_TRUE;

  printf("agent compute set symmetric difference: enter\n");

  if (!sc_memory_get_arc_end(s_utils_ctx, arg, &question))
    return SC_RESULT_ERROR_INVALID_PARAMS;

  // check question type
  if (sc_helper_check_arc(
          s_utils_ctx, keynode_question_compute_set_symmetric_difference, question, sc_type_arc_pos_const_perm) == SC_FALSE)
    return SC_RESULT_ERROR_INVALID_TYPE;

  printf("agent compute set symmetric difference: started\n");

  answer = create_answer_node();
  //SYSTEM_ELEMENT(s_utils_ctx,front);
  // get operation argument
  it51 = sc_iterator5_f_a_a_a_f_new(s_utils_ctx, question, sc_type_arc_pos_const_perm, sc_type_const, sc_type_arc_pos_const_perm, keynode_rrel_1);
  it52 = sc_iterator5_f_a_a_a_f_new(s_utils_ctx, question, sc_type_arc_pos_const_perm, sc_type_const, sc_type_arc_pos_const_perm, keynode_rrel_2);


  if (sc_iterator5_next(it51) == SC_TRUE) {
    if (sc_helper_check_arc(
          s_utils_ctx, keynode_set, sc_iterator5_value(it51,2), sc_type_arc_pos_const_actual) == SC_FALSE)
      return SC_RESULT_ERROR_INVALID_TYPE;

  if (sc_iterator5_next(it52) == SC_TRUE) {
    if (sc_helper_check_arc(
          s_utils_ctx, keynode_set, sc_iterator5_value(it52,2), sc_type_arc_pos_const_actual) == SC_FALSE)
      return SC_RESULT_ERROR_INVALID_TYPE;

    printf("agent compute set symmetric difference: found elements\n");

    sc_addr set1 = sc_iterator5_value(it51,2);
    sc_addr set2 = sc_iterator5_value(it52,2);

    sc_iterator5_free(it51);
    sc_iterator5_free(it52);

    sc_addr diff = SC_ADDR_EMPTY;

    it1 = sc_iterator3_f_a_f_new(s_utils_ctx, set1, sc_type_arc_const, set2);

    bool d = false;
    while (!d && (sc_iterator3_next(it1) == SC_TRUE)) {
       it51 = sc_iterator5_f_a_a_a_f_new(s_utils_ctx, sc_iterator3_value(it1,1), sc_type_arc_const, sc_type_const, sc_type_arc_pos_const_perm,keynode_set_symmetric_difference);
       if (sc_iterator5_next(it51) == SC_TRUE) {
         d = true;
         diff = sc_iterator5_value(it51,2);
       }
       sc_iterator5_free(it51);
    }

    sc_iterator3_free(it1);

    it1 = sc_iterator3_f_a_f_new(s_utils_ctx, set2, sc_type_arc_const, set1);

    while (!d && (sc_iterator3_next(it1) == SC_TRUE)) {
       it51 = sc_iterator5_f_a_a_a_f_new(s_utils_ctx, sc_iterator3_value(it1,1), sc_type_arc_const, sc_type_const, sc_type_arc_pos_const_perm,keynode_set_symmetric_difference);
       if (sc_iterator5_next(it51) == SC_TRUE) {
         d = true;
         diff = sc_iterator5_value(it51,2);
       }
       sc_iterator5_free(it51);
    }

    sc_iterator3_free(it1);

    if (!d) diff = sc_memory_node_new(s_utils_ctx,sc_type_const);

    appendIntoAnswer(answer, diff);


    if (sc_helper_check_arc(
        s_utils_ctx, keynode_exact_represented, set1, sc_type_arc_pos_const_actual) == SC_TRUE) {
      if (sc_helper_check_arc(
          s_utils_ctx, keynode_exact_represented, set2, sc_type_arc_pos_const_actual) == SC_TRUE) {

        printf("agent compute set symmetric difference: while started\n");

        it1 = sc_iterator3_f_a_a_new(s_utils_ctx, set1, sc_type_arc_pos_const_perm, 0);
        while (sc_iterator3_next(it1) == SC_TRUE) {
          it2 = sc_iterator3_f_a_f_new(s_utils_ctx, set2, sc_type_arc_neg_const_perm, sc_iterator3_value(it1,2));
          if (sc_iterator3_next(it2) == SC_TRUE) {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_pos_const_perm);
	      appendIntoAnswer(answer, arc);
          }
	  else {
	    sc_iterator3_free(it2);
	    it2 = sc_iterator3_f_a_f_new(s_utils_ctx, set2, sc_type_arc_pos_const_perm, sc_iterator3_value(it1,2));
	    if (sc_iterator3_next(it2) == SC_TRUE) {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_neg_const_perm);
    	      appendIntoAnswer(answer, arc);
	    }
	    else {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_pos_const_perm);
    	      appendIntoAnswer(answer, arc);
              printf("agent compute set symmetric difference: set1 element added\n");
	    }
	  }
          sc_iterator3_free(it2);
	}
        sc_iterator3_free(it1);

        it1 = sc_iterator3_f_a_a_new(s_utils_ctx, set2, sc_type_arc_pos_const_perm, 0);
        while (sc_iterator3_next(it1) == SC_TRUE) {
          it2 = sc_iterator3_f_a_f_new(s_utils_ctx, set1, sc_type_arc_neg_const_perm, sc_iterator3_value(it1,2));
          if (sc_iterator3_next(it2) == SC_TRUE) {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_pos_const_perm);
    	      appendIntoAnswer(answer, arc);
	  }
	  else {
	    sc_iterator3_free(it2);
	    it2 = sc_iterator3_f_a_f_new(s_utils_ctx, set1, sc_type_arc_pos_const_perm, sc_iterator3_value(it1,2));
	    if (sc_iterator3_next(it2) == SC_TRUE) {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_neg_const_perm);
//	      appendIntoAnswer(answer, arc);
	    }
	    else {
              sc_addr arc = updateArc(s_utils_ctx,diff,sc_iterator3_value(it1,2),sc_type_arc_pos_const_perm);
    	      appendIntoAnswer(answer, arc);
              printf("agent compute set symmetric difference: set2 element added\n");
	    }
	  }
          sc_iterator3_free(it2);
	}
        sc_iterator3_free(it1);

        sc_addr arc = updateArc(s_utils_ctx,keynode_exact_represented,diff,sc_type_arc_pos_const_temp);
        appendIntoAnswer(answer, arc);
        appendIntoAnswer(answer, keynode_exact_represented);
      }
      else {
      }
    }
    else {
    }

    //add structure
    sc_addr arc = updateArc(s_utils_ctx,set1,set2,sc_type_arc_fuz_const);
    appendIntoAnswer(answer, arc);
    arc = updateArc(s_utils_ctx,arc,diff,sc_type_arc_fuz_const);
    appendIntoAnswer(answer, arc);
    arc = updateArc(s_utils_ctx,keynode_set_symmetric_difference,arc,sc_type_arc_pos_const_perm);
    appendIntoAnswer(answer, arc);
    arc = updateArc(s_utils_ctx,keynode_set,diff,sc_type_arc_pos_const_perm);
    appendIntoAnswer(answer, keynode_set_symmetric_difference);
  }}


  connect_answer_to_question(question, answer);
  finish_question(question);

  printf("agent compute set symmetric difference: leave\n");

  return SC_RESULT_OK;
}
