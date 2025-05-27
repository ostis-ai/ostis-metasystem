/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

 #include "keynodes/translation_keynodes.hpp"

#include "constants/translation_constants.hpp"

#include "StructureTranslator.hpp"

namespace structureTranslationModule
{

size_t StructureTranslator::hashCode() const
{
  return std::hash<std::string>()(typeid(*this).name());
}

bool StructureTranslator::operator<(StructureTranslator const & other) const
{
  return this->hashCode() < other.hashCode();
}

bool StructureTranslator::isInStructure(ScAddr const & structAddr, ScAddr const & elementAddr) const
{
  return elementAddr == structAddr
         or (context->GetElementType(elementAddr) == ScType::EdgeAccessConstPosPerm
             and context->GetEdgeSource(elementAddr) == structAddr)
         or context->HelperCheckEdge(structAddr, elementAddr, ScType::EdgeAccessConstPosPerm);
}

bool StructureTranslator::isIgnored(ScAddr const & nodeAddr) const
{
  auto const & ignoredIterator = context->Iterator3(
      TranslationKeynodes::translation_ignored_keynodes, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (ignoredIterator->Next())
  {
    ScAddr const & ignoredKeynode = ignoredIterator->Get(2);
    if (context->HelperCheckEdge(nodeAddr, ignoredKeynode, ScType::EdgeAccessConstPosPerm))
      return true;
    if (context->HelperCheckEdge(ignoredKeynode, nodeAddr, ScType::EdgeAccessConstPosPerm))
      return true;
  }
  return false;
}

}  // namespace structureTranslationModule
