/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "container_utils.hpp"

namespace verificationModule
{
void ContainerUtils::InsertSeveral(ScAddrUnorderedSet & set, std::initializer_list<ScAddr> const & elements)
{
  set.insert(elements.begin(), elements.end());
}

}  // namespace verificationModule
