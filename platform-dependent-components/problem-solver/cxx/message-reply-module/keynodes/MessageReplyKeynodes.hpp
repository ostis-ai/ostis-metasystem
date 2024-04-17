/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "MessageReplyKeynodes.generated.hpp"

namespace messageReplyModule
{
class MessageReplyKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_reply_to_message"), ForceCreate)
  static ScAddr action_reply_to_message;

  SC_PROPERTY(Keynode("nrel_authors"), ForceCreate)
  static ScAddr nrel_authors;

  SC_PROPERTY(Keynode("nrel_reply"), ForceCreate)
  static ScAddr nrel_reply;

  SC_PROPERTY(Keynode("action_find_identifiers"), ForceCreate)
  static ScAddr myself;

  SC_PROPERTY(Keynode("message_processing_program"), ForceCreate)
  static ScAddr message_processing_program;

  SC_PROPERTY(Keynode("languages"), ForceCreate)
  static ScAddr languages;

  SC_PROPERTY(Keynode("concept_text_file"), ForceCreate)
  static ScAddr concept_text_file;

  SC_PROPERTY(Keynode("concept_message"), ForceCreate)
  static ScAddr concept_message;

  SC_PROPERTY(Keynode("action_interpret_non_atomic_action"), ForceCreate)
  static ScAddr action_interpret_non_atomic_action;
};

}  // namespace messageReplyModule
