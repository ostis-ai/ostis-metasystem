/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>
#include "agent/check_homonyms_agent.hpp"

using namespace verificationModule;

namespace CheckHomonymsAgentTest
{
class CheckHomonymsAgentTest : public testing::Test
{
public:
  static inline std::string const & TEST_FILES_DIR_PATH = VERIFICATION_MODULE_TEST_SRC_PATH "/testStructures/homonyms/";
  static inline std::string const & TEST_KB_BIN_PATH = "../kb.bin/";
  static inline int const WAIT_TIME = 1000;
  static inline ScsLoader loader;

protected:
  std::unique_ptr<ScAgentContext> m_ctx;

  virtual void SetUp()
  {
    CheckHomonymsAgentTest::Initialize();
    m_ctx = std::make_unique<ScAgentContext>();
  }

  virtual void TearDown()
  {
    if (m_ctx)
      m_ctx->Destroy();

    CheckHomonymsAgentTest::Shutdown();

    // todo(kilativ-dotcom): uncomment after using build system
    // std::filesystem::remove_all(TEST_KB_BIN_PATH);
  }

  static void Initialize()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    // todo(kilativ-dotcom): use ScMemory::ms_configPath after using build system
    // ScMemory::ms_configPath = "../../../../../../ostis-metasystem.ini";
    params.dump_memory = false;
    params.dump_memory_statistics = false;

    params.clear = false;
    params.storage = TEST_KB_BIN_PATH.c_str();

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(SC_FALSE);
    ScMemory::LogUnmute();
  }
};

TEST_F(CheckHomonymsAgentTest, TwoHomonymsTest)
{
  ScAgentContext & context = *m_ctx;

  SC_LOG_INFO("reading " << std::filesystem::absolute(TEST_FILES_DIR_PATH + "twoHomonyms.scs").c_str());
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "twoHomonyms.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(context.IsElement(testActionNode));

  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<CheckHomonymsAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<CheckHomonymsAgent>();
}

}  // namespace CheckHomonymsAgentTest



