#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace messageClassificationModule
{
class MessageSearcher
{
public:
  explicit MessageSearcher(ScMemoryContext * ms_context);

  ScAddr getFirstMessage(ScAddr const & nonAtomicMessageNode);

  ScAddr getNextMessage(ScAddr const & messageNode);

  ScAddr getMessageAuthor(ScAddr const & messageNode);

  ScAddr getMessageTheme(ScAddr const & messageNode);

  ScAddrVector getMessageLinks(ScAddr const & message, ScAddrVector const & linkClasses = {});

  ScAddr getMessageLink(ScAddr const & message, ScAddrVector const & linkClasses = {});

private:
  ScMemoryContext * context;
};
}  // namespace messageClassificationModule
