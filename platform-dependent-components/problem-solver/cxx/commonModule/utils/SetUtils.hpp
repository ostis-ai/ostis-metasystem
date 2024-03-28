/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Ruslan Korshunov
 */

#include "sc-memory/sc_memory.hpp"

namespace commonModule
{
class SetUtils
{
public:
  static ScAddrVector getAllElementsByEdgeType(ScMemoryContext * context, ScAddr const &, ScType edgeType);

  static ScAddr getEdge(ScMemoryContext *, ScAddr const &, ScAddr const &);

  static void addToSets(ScMemoryContext *, ScAddr const &, ScAddrVector const &);

  static void removeFromSets(ScMemoryContext *, ScAddr const &, ScAddrVector const &);

  static void removeBaseEdgesFromSet(ScMemoryContext * context, ScAddr const & set);
};
} // namespace commonModule
