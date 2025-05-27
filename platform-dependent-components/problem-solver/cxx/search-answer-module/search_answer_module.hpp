/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_module.hpp"

#include "keynodes/search_answer_keynodes.hpp"

namespace searchAnswerModule
{

class SearchAnswerModule : public ScModule
{
    void Initialize(ScMemoryContext * m_context) override;
    void Shutdown(ScMemoryContext * m_context) override;
};

}  // namespace searchAnswerModule
