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
         or (context->GetElementType(elementAddr) == ScType::ConstPermPosArc
             and context->GetArcSourceElement(elementAddr) == structAddr)
         or context->CheckConnector(structAddr, elementAddr, ScType::ConstPermPosArc);
}

bool StructureTranslator::isIgnored(ScAddr const & nodeAddr) const
{
  auto const & ignoredIterator = context->CreateIterator3(
      TranslationKeynodes::translation_ignored_keynodes, ScType::ConstPermPosArc, ScType::ConstNode);
  while (ignoredIterator->Next())
  {
    ScAddr const & ignoredKeynode = ignoredIterator->Get(2);
    if (context->CheckConnector(nodeAddr, ignoredKeynode, ScType::ConstPermPosArc))
      return true;
    if (context->CheckConnector(ignoredKeynode, nodeAddr, ScType::ConstPermPosArc))
      return true;
  }
  return false;
}

}  // namespace structureTranslationModule
