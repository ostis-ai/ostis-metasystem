/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/test/sc_test.hpp>

#include <memory>
#include <algorithm>
#include <unordered_set>
#include <filesystem>

#include <sc-builder/scs_loader.hpp>
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

class DuplicatingConstructionCheckAgentTest : public ScMemoryTest
{
public:
 //static inline std::string const & TEST_FILES_DIR_PATH = "../test-structures/";
 static inline std::string const & TEST_FILES_DIR_PATH = VERIFICATION_MODULE_TEST_SRC_PATH "/testStructures/";
 static inline std::string const & TEST_KB_BIN_PATH = "../kb.bin/";
 //временно абсолютный, после ребейза на новую билд систему - преехатц
 static inline std::string const & TEST_CONFIG_PATH =
     "/home/gray/Work/Projects/ostis-metasystem/platform-dependent-components/problem-solver/cxx/verification-module/test/test-config.ini";
 static inline int const WAIT_TIME = 5000;
 static inline ScsLoader loader;

protected:
 virtual void SetUp()
 {
   DuplicatingConstructionCheckAgentTest::Initialize();
   m_ctx = std::make_unique<ScAgentContext>();
 }

 virtual void TearDown()
 {
   if (m_ctx)
     m_ctx->Destroy();

   DuplicatingConstructionCheckAgentTest::Shutdown();

   std::filesystem::remove_all(TEST_KB_BIN_PATH);

//   //нужно ли?
//   std::filesystem::remove_all(TEST_KB_BIN_PATH);
 }

 static void Initialize()
 {
   sc_memory_params params;
   sc_memory_params_clear(&params);

   params.dump_memory = false;
   params.dump_memory_statistics = false;

   params.clear = true;
   params.storage = TEST_KB_BIN_PATH.c_str();

   ScMemory::ms_configPath = TEST_CONFIG_PATH;

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
