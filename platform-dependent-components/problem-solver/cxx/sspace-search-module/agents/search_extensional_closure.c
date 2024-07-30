/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_extensional_closure.h"
#include "search_keynodes.h"
#include "search_utils.h"
#include "search_defines.h"
#include "search.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
#include <stdbool.h>

#define sc_type_arc_pos_const_temp (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos|sc_type_arc_temp)
#define sc_type_arc_pos_const (sc_type)(sc_type_const|sc_type_arc_access|sc_type_arc_pos)

sc_result agent_search_extensional_closure(const sc_event * event, sc_addr arg)
{
  sc_addr question, answer;
  sc_iterator3 *it1, *it2;
  sc_bool sys_off = SC_TRUE;
  bool er = true;

  printf("agent search extensional closure: enter\n");

  if (!sc_memory_get_arc_end(s_default_ctx, arg, &question))
    return SC_RESULT_ERROR_INVALID_PARAMS;

  // check question type
  if (sc_helper_check_arc(
          s_default_ctx, keynode_question_extensional_closure, question, sc_type_arc_pos_const_perm) == SC_FALSE)
    return SC_RESULT_ERROR_INVALID_TYPE;

  printf("agent search extensional closure: started\n");

  answer = create_answer_node();
  sc_addr front, back = sc_memory_node_new(s_default_ctx,sc_type_const);
  SYSTEM_ELEMENT(back);
  // get operation argument
  it1 = sc_iterator3_f_a_a_new(s_default_ctx, question, sc_type_arc_pos_const_perm, 0);

  if (sc_iterator3_next(it1) == SC_TRUE) {
    printf("agent search extensional closure: found element\n");
    if (sc_helper_check_arc(s_default_ctx,back,sc_iterator3_value(it1,2),sc_type_arc_pos_const_temp)!=SC_TRUE) {
      sc_addr arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_temp,back,sc_iterator3_value(it1,2));
      SYSTEM_ELEMENT(arc);
    }

    sc_iterator3_free(it1);

    bool f = true;
    while(f){
      printf("agent search extensional closure: while started\n");
      f = false;

      front = sc_memory_node_new(s_default_ctx,sc_type_const);
      SYSTEM_ELEMENT(front);

      it1 = sc_iterator3_f_a_a_new(s_default_ctx, back, sc_type_arc_pos_const_temp, 0);
      while (sc_iterator3_next(it1) == SC_TRUE)
      {
//        if (IS_SYSTEM_ELEMENT(sc_iterator3_value(it1, 2)))
//          sys_off = SC_FALSE;

        appendIntoAnswer(answer, sc_iterator3_value(it1, 2));
      // iterate output arcs and append them into answer
        it2 = sc_iterator3_f_a_a_new(s_default_ctx, sc_iterator3_value(it1, 2), sc_type_arc_pos, 0);
        while (sc_iterator3_next(it2) == SC_TRUE)
        {
//          if (sys_off == SC_TRUE &&
//              (IS_SYSTEM_ELEMENT(sc_iterator3_value(it2, 1)) || IS_SYSTEM_ELEMENT(sc_iterator3_value(it2, 2))))
//            continue;

/*          if ((sc_helper_check_arc(s_default_ctx,answer,sc_iterator3_value(it2,1),sc_type_arc_pos_const_perm)!=SC_TRUE) &&
             (sc_helper_check_arc(s_default_ctx,front,sc_iterator3_value(it2,1),sc_type_arc_pos_const_temp)!=SC_TRUE)) {
            sc_addr arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_temp,front,sc_iterator3_value(it2,1));
            SYSTEM_ELEMENT(arc);
            f = true;
          }
*/          if ((sc_helper_check_arc(s_default_ctx,answer,sc_iterator3_value(it2,2),sc_type_arc_pos_const_perm)!=SC_TRUE) &&
             (sc_helper_check_arc(s_default_ctx,front,sc_iterator3_value(it2,2),sc_type_arc_pos_const_temp)!=SC_TRUE)) {
            sc_addr arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_temp,front,sc_iterator3_value(it2,2));
            SYSTEM_ELEMENT(arc);
            f = true;
//            appendIntoAnswer(answer, sc_iterator3_value(it2, 1));
            appendIntoAnswer(answer, sc_iterator3_value(it2, 2));
            printf("agent search extensional closure: element added\n");
          }
      
        }    
        sc_iterator3_free(it2);

        it2 = sc_iterator3_a_f_a_new(s_default_ctx, 0, sc_iterator3_value(it1, 2), 0);
        if (sc_iterator3_next(it2) == SC_TRUE) {
          if ((sc_helper_check_arc(s_default_ctx,answer,sc_iterator3_value(it2,0),sc_type_arc_pos_const_perm)!=SC_TRUE) &&
             (sc_helper_check_arc(s_default_ctx,front,sc_iterator3_value(it2,0),sc_type_arc_pos_const_temp)!=SC_TRUE)) {
            sc_addr arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_temp,front,sc_iterator3_value(it2,0));
            SYSTEM_ELEMENT(arc);
            f = true;
            appendIntoAnswer(answer, sc_iterator3_value(it2, 0));
            printf("agent search extensional closure: beginning added\n");
          }
          if ((sc_helper_check_arc(s_default_ctx,answer,sc_iterator3_value(it2,2),sc_type_arc_pos_const_perm)!=SC_TRUE) &&
             (sc_helper_check_arc(s_default_ctx,front,sc_iterator3_value(it2,2),sc_type_arc_pos_const_temp)!=SC_TRUE)) {
            sc_addr arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_temp,front,sc_iterator3_value(it2,2));
            SYSTEM_ELEMENT(arc);
            f = true;
            appendIntoAnswer(answer, sc_iterator3_value(it2, 2));
            printf("agent search extensional closure: ending added\n");
          }

        }
        sc_iterator3_free(it2);

      }
      sc_memory_element_free(s_default_ctx,back);
      back = front;
    }
    sc_iterator3_free(it1);
  } else
    sc_iterator3_free(it1);

  sc_memory_element_free(s_default_ctx,back);
  connect_answer_to_question(question, answer);
  finish_question(question);

  printf("agent search extensional closure: leave\n");

  return SC_RESULT_OK;
}

