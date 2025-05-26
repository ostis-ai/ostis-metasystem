#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

#include "classifier/MessageTopicClassifier.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationManager
{
public:
  explicit MessageTopicClassificationManager(ScMemoryContext * context);

  ScAddrVector manage(ScAddrVector const & processParameters) const;

protected:
  std::unique_ptr<MessageTopicClassifier> classifier;
};

}  // namespace messageClassificationModule
