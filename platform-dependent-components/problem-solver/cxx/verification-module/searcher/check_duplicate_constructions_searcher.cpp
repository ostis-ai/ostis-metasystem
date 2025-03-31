/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "filesystem"
#include "optional"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "constants/verification_constants.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "check_duplicate_constructions_searcher.hpp"

namespace verificationModule
{
checkDuplicateConstructionSearcher::checkDuplicateConstructionSearcher(ScMemoryContext * context)
  : context(context)
{
}

}  // namespace verificationModule
