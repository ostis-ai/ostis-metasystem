/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "utils_utils.h"
#include "utils_keynodes.h"
#include "utils_defines.h"
#include "utils.h"
//#include "sc-core/sc-store/sc-base/sc_message.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"

sc_addr create_answer_node()
{
  sc_addr res = sc_memory_node_new(s_utils_ctx, sc_type_const);
  SYSTEM_ELEMENT(s_utils_ctx,res);
  return res;
}

void connect_answer_to_question(sc_addr question, sc_addr answer)
{
  sc_addr arc;

  arc = sc_memory_arc_new(s_utils_ctx, sc_type_arc_common | sc_type_const, question, answer);
  SYSTEM_ELEMENT(s_utils_ctx,arc);
  arc = sc_memory_arc_new(s_utils_ctx, sc_type_arc_pos_const_perm, keynode_nrel_answer, arc);
  SYSTEM_ELEMENT(s_utils_ctx,arc);
}

void appendIntoAnswer(sc_addr answer, sc_addr el)
{
  sc_addr arc;
  if (sc_helper_check_arc(s_utils_ctx, answer, el, sc_type_arc_pos_const_perm) == SC_TRUE)
    return;

  arc = sc_memory_arc_new(s_utils_ctx, sc_type_arc_pos_const_perm, answer, el);
  SYSTEM_ELEMENT(s_utils_ctx,arc);
}

sc_addr updateArc(sc_memory_context * ctx, sc_addr beg, sc_addr end, sc_type arc_type)
{
  sc_addr arc = SC_ADDR_EMPTY;
  sc_iterator3  * it = sc_iterator3_f_a_f_new(ctx,beg,arc_type,end);

  if (it == null_ptr)
    return arc;

  if (sc_iterator3_next(it) == SC_TRUE) {
    arc = sc_iterator3_value(it,1);
    sc_iterator3_free(it);
    return arc;
  } 

  sc_iterator3_free(it);
  return sc_memory_arc_new(ctx, arc_type, beg, end);
}

void finish_question(sc_addr question)
{
  sc_addr arc;

  arc = sc_memory_arc_new(s_utils_ctx, sc_type_arc_pos_const_perm, keynode_question_finished, question);
  SYSTEM_ELEMENT(s_utils_ctx,arc);
}

void finish_question_successfully(sc_memory_context * ctx, sc_addr question)
{
  sc_memory_arc_new(ctx, sc_type_arc_pos_const_perm, keynode_question_finished_successfully, question);
}

void finish_question_unsuccessfully(sc_memory_context * ctx, sc_addr question)
{
  sc_memory_arc_new(ctx, sc_type_arc_pos_const_perm, keynode_question_finished_unsuccessfully, question);
}
