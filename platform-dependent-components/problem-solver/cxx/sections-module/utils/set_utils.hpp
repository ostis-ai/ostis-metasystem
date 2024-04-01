/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_memory.hpp"

namespace sectionsModule
{
class SetUtils
{
public:
  static ScAddrVector GetAllElementsByEdgeType(ScMemoryContext * context, ScAddr const &, ScType edgeType);

  static ScAddr GetEdge(ScMemoryContext *, ScAddr const &, ScAddr const &);

  static void AddToSets(ScMemoryContext *, ScAddr const &, ScAddrVector const &);

  static void RemoveFromSets(ScMemoryContext *, ScAddr const &, ScAddrVector const &);

  static void RemoveBaseEdgesFromSet(ScMemoryContext * context, ScAddr const & set);
};
}  // namespace subjectDomainModule
