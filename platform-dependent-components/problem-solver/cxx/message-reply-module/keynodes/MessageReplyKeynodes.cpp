/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MessageReplyKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace messageReplyModule
{
ScAddr MessageReplyKeynodes::action_reply_to_message;
ScAddr MessageReplyKeynodes::nrel_authors;
ScAddr MessageReplyKeynodes::nrel_reply;
ScAddr MessageReplyKeynodes::myself;
ScAddr MessageReplyKeynodes::message_processing_program;
ScAddr MessageReplyKeynodes::languages;
ScAddr MessageReplyKeynodes::concept_text_file;
ScAddr MessageReplyKeynodes::concept_message;
ScAddr MessageReplyKeynodes::action_interpret_non_atomic_action;
}  // namespace messageReplyModule
