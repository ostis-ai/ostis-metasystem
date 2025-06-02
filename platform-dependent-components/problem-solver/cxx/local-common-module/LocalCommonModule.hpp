#pragma once

#include "sc-memory/sc_module.hpp"

#include "keynodes/LocalKeynodes.hpp"

namespace commonModule
{
class LocalCommonModule : public ScModule
{
    void Initialize(ScMemoryContext * m_context) override;
    void Shutdown(ScMemoryContext * m_context) override;
};
}  // namespace commonModule
