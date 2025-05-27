/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_addr.hpp"

namespace structureTranslationModule
{
using ScAddrSet = ScAddrUnorderedSet;


class StructureTranslator
{
public:
  explicit StructureTranslator(ScMemoryContext * context)
    : context(context)
  {
  }

  virtual std::stringstream translate(ScAddr const & structAddr, ScAddr const & lang) const = 0;

  size_t hashCode() const;

  bool operator<(StructureTranslator const & other) const;

protected:
  ScMemoryContext * context;

  bool isInStructure(ScAddr const & structAddr, ScAddr const & elementAddr) const;

  bool isIgnored(ScAddr const & nodeAddr) const;
};

struct StructureTranslatorComporator
{
  bool operator()(StructureTranslator * first, StructureTranslator * second) const
  {
    return *first < *second;
  }
};

}  // namespace structureTranslationModule
