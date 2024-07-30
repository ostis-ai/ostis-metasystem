/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_extensional_closure_h_
#define _search_extensional_closure_h_

#include "sc-core/sc_memory.h"

/*!
 * Function that implement sc-agent to search all positive, constant and permanent
 * output arcs for specified sc-element
 */
sc_result agent_search_extensional_closure(const sc_event * event, sc_addr arg);

/*! Function that implemets sc-agent to search all positive, constant and permanent
 * output arcs with relations for specified sc-element
 */
//sc_result agent_search_all_const_pos_output_arc_with_rel(const sc_event * event, sc_addr arg);

#endif
