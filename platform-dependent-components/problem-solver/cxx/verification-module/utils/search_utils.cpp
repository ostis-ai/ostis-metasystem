/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_utils.hpp"

#include "keynodes/verification_keynodes.hpp"

namespace verificationModule
{
void SearchUtils::getSingularRelations(ScMemoryContext * context, ScAddrList & singularRelations)
{
  ScIterator3Ptr relationsIterator = context->CreateIterator3(
      VerificationKeynodes::concept_singular_relation, ScType::ConstPermPosArc, ScType::Unknown);

  while (relationsIterator->Next())
    singularRelations.push_back(relationsIterator->Get(0));
}

void SearchUtils::getQuasybinaryRelations(ScMemoryContext * context, ScAddrUnorderedSet & quasybinaryRelations)
{
  ScIterator3Ptr relationsIterator = context->CreateIterator3(
      VerificationKeynodes::quasybinary_relation, ScType::ConstPermPosArc, ScType::ConstNodeNonRole);

  while (relationsIterator->Next())
    quasybinaryRelations.insert(relationsIterator->Get(0));
}

}  // namespace verificationModule
