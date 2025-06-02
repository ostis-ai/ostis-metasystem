#include "MessageSearcher.hpp"

#include "sc-memory/sc_keynodes.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/message_keynodes.hpp"
#include <algorithm>

using namespace messageClassificationModule;

MessageSearcher::MessageSearcher(ScMemoryContext * ms_context)
{
  this->context = ms_context;
}

ScAddr MessageSearcher::getFirstMessage(ScAddr const & nonAtomicMessageNode)
{
  std::string const VAR_TUPLE = "_tuple";
  std::string const VAR_MESSAGE = "_message";
  ScTemplate templ;
  templ.Quintuple(
      ScType::NodeVarTuple >> VAR_TUPLE,
      ScType::EdgeDCommonVar,
      nonAtomicMessageNode,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_message_decomposition);
  templ.Quintuple(
      VAR_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> VAR_MESSAGE,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);

  ScAddr resultMessageNode;

  context->HelperSmartSearchTemplate(
      templ,
      [&resultMessageNode, &VAR_MESSAGE](ScTemplateResultItem const & resultItem)
      {
        resultMessageNode = resultItem[VAR_MESSAGE];
        SC_LOG_DEBUG("MessageSearcher: the first message node found");
        return ScTemplateSearchRequest::STOP;
      });

  if (!resultMessageNode.IsValid())
  {
    SC_LOG_DEBUG("MessageSearcher: the first message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getNextMessage(ScAddr const & messageNode)
{
  std::string const VAR_TUPLE = "_tuple";
  std::string const VAR_EDGE_1 = "_edge_1";
  std::string const VAR_EDGE_2 = "_edge_2";
  std::string const VAR_D_COMMON_EDGE = "_d_common_edge";
  std::string const VAR_MESSAGE = "_message";

  ScTemplate templ;
  templ.Triple(ScType::NodeVarTuple >> VAR_TUPLE, ScType::EdgeAccessVarPosPerm >> VAR_EDGE_1, messageNode);
  templ.Quintuple(
      VAR_EDGE_1,
      ScType::EdgeDCommonVar >> VAR_D_COMMON_EDGE,
      ScType::EdgeAccessVarPosPerm >> VAR_EDGE_2,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_message_sequence);
  templ.Triple(VAR_TUPLE, VAR_EDGE_2, ScType::NodeVar >> VAR_MESSAGE);

  ScAddr resultMessageNode;
  context->HelperSmartSearchTemplate(
      templ,
      [&resultMessageNode, &VAR_MESSAGE](ScTemplateResultItem const & resultItem)
      {
        resultMessageNode = resultItem[VAR_MESSAGE];
        SC_LOG_DEBUG("MessageSearcher: next message node found");
        return ScTemplateSearchRequest::STOP;
      });

  if (!resultMessageNode.IsValid())
  {
    SC_LOG_DEBUG("MessageSearcher: next message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getMessageAuthor(ScAddr const & messageNode)
{
  ScTemplate templ;
  std::string const VAR_AUTHOR = "_author";
  templ.Quintuple(
      messageNode,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> VAR_AUTHOR,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_authors);

  ScAddr resultAuthorNode;
  context->HelperSmartSearchTemplate(
      templ,
      [&resultAuthorNode, &VAR_AUTHOR](ScTemplateResultItem const & resultItem)
      {
        resultAuthorNode = resultItem[VAR_AUTHOR];
        SC_LOG_DEBUG("MessageSearcher: author set node found");

        return ScTemplateSearchRequest::STOP;
      });

  if (!resultAuthorNode.IsValid())
  {
    SC_LOG_DEBUG("MessageSearcher: author set node not found");
  }

  return resultAuthorNode;
}

ScAddr MessageSearcher::getMessageTheme(ScAddr const & messageNode)
{
  ScTemplate templ;
  std::string const VAR_THEME = "_theme";
  templ.Quintuple(
      messageNode,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> VAR_THEME,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::rrel_message_theme);

  ScAddr resultThemeNode;
  context->HelperSmartSearchTemplate(
      templ,
      [&resultThemeNode, &VAR_THEME](ScTemplateResultItem const & resultItem)
      {
        resultThemeNode = resultItem[VAR_THEME];
        SC_LOG_DEBUG("MessageSearcher: message theme node found");

        return ScTemplateSearchRequest::STOP;
      });

  if (!resultThemeNode.IsValid())
  {
    SC_LOG_DEBUG("MessageSearcher: message theme node not found");
  }

  return resultThemeNode;
}

ScAddrVector MessageSearcher::getMessageLinks(ScAddr const & message, ScAddrVector const & linkClasses)
{
  ScAddrVector messageLinks;
  ScAddr const translationNode = utils::IteratorUtils::getAnyByInRelation(
      context, message, MessageKeynodes::nrel_sc_text_translation);
  if (!translationNode.IsValid())
  {
    SC_LOG_WARNING("MessageSearcher: text translation node not found");
    return {};
  }

  ScIterator3Ptr const linkIterator =
      context->Iterator3(translationNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
  while (linkIterator->Next())
  {
    ScAddr const & linkAddr = linkIterator->Get(2);
    bool result = std::all_of(
        linkClasses.cbegin(),
        linkClasses.cend(),
        [this, &linkAddr](auto const & addr)
        {
          return context->HelperCheckEdge(addr, linkAddr, ScType::EdgeAccessConstPosPerm);
        });

    if (result == SC_TRUE)
    {
      messageLinks.push_back(linkAddr);
    }
  }
  return messageLinks;
}

ScAddr MessageSearcher::getMessageLink(ScAddr const & message, ScAddrVector const & linkClasses)
{
  ScAddr messageLink;
  ScAddrVector messageLinks = getMessageLinks(message, linkClasses);
  if (!messageLinks.empty())
    messageLink = messageLinks.at(0);
  return messageLink;
}
