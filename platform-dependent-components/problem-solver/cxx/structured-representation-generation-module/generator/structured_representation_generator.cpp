#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "keynodes/structured_representation_generation_keynodes.hpp"
#include "utils/template_utils.hpp"

#include "structured_representation_generator.hpp"

using namespace structuredRepresentationGenerationModule;

StructuredRepresentationGenerator::StructuredRepresentationGenerator(ScMemoryContext * context)
  : context(context)
{
}

ScAddrVector StructuredRepresentationGenerator::GenerateStructuredRepresentation(
    ScAddr const & structure,
    ScAddr const & rule)
{
  ScAddr startElement =
      utils::IteratorUtils::getAnyByOutRelation(context, structure, scAgentsCommon::CoreKeynodes::rrel_key_sc_element);
  GenerateStructuredRepresentation(structure, rule, startElement);

  return std::move(currentGeneratedElements);
}

ScAddr StructuredRepresentationGenerator::GenerateStructuredRepresentation(
    ScAddr const & structure,
    ScAddr const & rule,
    ScAddr const & startElement)
{
  ScAddr const keyVariable =
      utils::IteratorUtils::getAnyByOutRelation(context, rule, scAgentsCommon::CoreKeynodes::rrel_key_sc_element);

  ScAddr currentAddr = utils::IteratorUtils::getAnyByOutRelation(context, rule, scAgentsCommon::CoreKeynodes::rrel_1);

  ScAddr nextElement;

  ScAddr const resultTuple = context->CreateNode(ScType::NodeConstTuple);

  currentGeneratedElements.emplace_back(resultTuple);

  while (context->IsElement(currentAddr))
  {
    nextElement = utils::IteratorUtils::getNextFromSet(context, rule, currentAddr);

    ScAddrVector currentStructures;
    if (context->IsElement(nextElement) && context->GetElementType(nextElement) == ScType::NodeConstTuple)
    {
      currentStructures =
          ProcessTemplateWithAdditionRule(structure, currentAddr, keyVariable, nextElement, startElement);
      currentAddr = nextElement;
    }
    else if (!context->IsElement(nextElement) || context->GetElementType(nextElement) == ScType::NodeConstStruct)
    {
      currentStructures = ProcessTemplate(structure, currentAddr, keyVariable, startElement);
    }

    std::for_each(
        currentStructures.begin(),
        currentStructures.end(),
        [this, resultTuple](ScAddr const & currentAddr)
        {
          currentGeneratedElements.emplace_back(currentAddr);
          AddElementToTuple(currentAddr, resultTuple);
        });

    if (currentAddr != nextElement)
    {
      currentAddr = nextElement;
      continue;
    }

    currentAddr = utils::IteratorUtils::getNextFromSet(context, rule, currentAddr);
  }
  return resultTuple;
}

ScAddrVector StructuredRepresentationGenerator::ProcessTemplate(
    ScAddr const & structure,
    ScAddr const & templ,
    ScAddr const & templateKeyVariable,
    ScAddr const & startElement)
{
  ScTemplateParams params;
  params.Add(templateKeyVariable, startElement);

  ScTemplate buildedTempl;
  context->HelperBuildTemplate(buildedTempl, templ, params);

  ScAddrVector resultVector;
  context->HelperSmartSearchTemplate(
      buildedTempl,
      [this, &structure, &resultVector](ScTemplateResultItem const & resultItem)
      {
        if (!TemplateUtils::CheckAllSearchResultItemInStructure(context, resultItem, structure))
          return ScTemplateSearchRequest::CONTINUE;

        ScAddr generatedStructure = WrapInStructure(resultItem);
        resultVector.emplace_back(generatedStructure);
        return ScTemplateSearchRequest::CONTINUE;
      });
  return resultVector;
}

ScAddrVector StructuredRepresentationGenerator::ProcessTemplateWithAdditionRule(
    ScAddr const & structure,
    ScAddr const & templ,
    ScAddr const & templateKeyVariable,
    ScAddr const & additionRule,
    ScAddr const & startElement)
{
  ScTemplateParams params;
  params.Add(templateKeyVariable, startElement);

  ScTemplate buildedTempl;
  context->HelperBuildTemplate(buildedTempl, templ, params);

  ScAddr additionKeyVariable = utils::IteratorUtils::getAnyByOutRelation(
      context, additionRule, scAgentsCommon::CoreKeynodes::rrel_key_sc_element);

  ScAddrVector resultVector;
  context->HelperSmartSearchTemplate(
      buildedTempl,
      [this, &resultVector, &structure, &additionKeyVariable, &additionRule](ScTemplateResultItem const & resultItem)
      {
        if (!TemplateUtils::CheckAllSearchResultItemInStructure(context, resultItem, structure))
          return ScTemplateSearchRequest::CONTINUE;
        ScAddr generatedStructure = WrapInStructure(resultItem);
        resultVector.emplace_back(generatedStructure);

        ScAddr additionKeynode;
        if (!resultItem.Get(additionKeyVariable, additionKeynode))
          return ScTemplateSearchRequest::CONTINUE;

        ScAddr additionResult = GenerateStructuredRepresentation(structure, additionRule, additionKeynode);
        resultVector.emplace_back(additionResult);

        return ScTemplateSearchRequest::CONTINUE;
      });
  return resultVector;
}

ScAddr StructuredRepresentationGenerator::WrapInStructure(ScTemplateResultItem const & resultItem)
{
  ScAddr structureAddr = context->CreateNode(ScType::NodeConstStruct);

  for (auto const & currentResultItem : resultItem)
  {
    context->CreateEdge(ScType::EdgeAccessConstPosPerm, structureAddr, currentResultItem);
  }

  return structureAddr;
}

void StructuredRepresentationGenerator::AddElementToTuple(ScAddr const & element, ScAddr const & tuple)
{
  ScIterator5Ptr tupleIterator = context->Iterator5(
      tuple,
      ScType::EdgeAccessConstPosPerm,
      ScType::Unknown,
      ScType::EdgeAccessConstPosTemp,
      StructuredRepresentationGenerationKeynodes::rrel_last);

  if (!tupleIterator->Next())
  {
    ScAddr elementEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, tuple, element);
    context->CreateEdge(
        ScType::EdgeAccessConstPosTemp, StructuredRepresentationGenerationKeynodes::rrel_last, elementEdge);
    ScAddr rrel1Edge =
        context->CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::rrel_1, elementEdge);

    currentGeneratedElements.emplace_back(elementEdge);
    currentGeneratedElements.emplace_back(rrel1Edge);
    currentGeneratedElements.emplace_back(scAgentsCommon::CoreKeynodes::rrel_1);
    return;
  }

  context->EraseElement(tupleIterator->Get(3));
  ScAddr elementEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, tuple, element);
  context->CreateEdge(
      ScType::EdgeAccessConstPosTemp, StructuredRepresentationGenerationKeynodes::rrel_last, elementEdge);
  ScAddr sequenceEdge = context->CreateEdge(ScType::EdgeDCommonConst, tupleIterator->Get(1), elementEdge);
  ScAddr nrelSequenceEdge = context->CreateEdge(
      ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::nrel_basic_sequence, sequenceEdge);

  currentGeneratedElements.emplace_back(elementEdge);
  currentGeneratedElements.emplace_back(sequenceEdge);
  currentGeneratedElements.emplace_back(nrelSequenceEdge);
  currentGeneratedElements.emplace_back(scAgentsCommon::CoreKeynodes::nrel_basic_sequence);
}
