#!/usr/bin/env bash
set -eo pipefail

ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export METASYSTEM_PATH="${METASYSTEM_PATH:-${ROOT_PATH}}"

export METASYSTEM_CMAKE_PATH="${METASYSTEM_CMAKE_PATH:-${ROOT_PATH}}"
export METASYSTEM_CXX_SOURCES_PATH="${METASYSTEM_PATH}/platform-dependent-components/problem-solver"
export METASYSTEM_BUILD_PATH="${METASYSTEM_BUILD_PATH:-${METASYSTEM_CMAKE_PATH}/build}"
export BINARY_PATH="${BINARY_PATH:-${METASYSTEM_CMAKE_PATH}/bin}"

export CONFIG_PATH="${CONFIG_PATH:-${METASYSTEM_PATH}/ostis-metasystem.ini}"
export REPO_PATH_FILE="${REPO_PATH_FILE:-${METASYSTEM_PATH}/repo.path}"

export PLATFORM_REPO="${PLATFORM_REPO:-https://github.com/NikitaZotov/ostis-web-platform.git}"
export PLATFORM_BRANCH="${PLATFORM_BRANCH:-feat/move_common_scripts}"
export PLATFORM_COMMIT="${PLATFORM_COMMIT:-""}"
export PLATFORM_PATH="${PLATFORM_PATH:-${METASYSTEM_PATH}/ostis-web-platform}"

export SC_MACHINE_REPO="${SC_MACHINE_REPO:-https://github.com/NikitaZotov/sc-machine.git}"
export SC_MACHINE_BRANCH="${SC_MACHINE_BRANCH:-feat/move_common_scripts}"
export SC_MACHINE_COMMIT="${SC_MACHINE_COMMIT:-""}"

export SCP_MACHINE_REPO="${SCP_MACHINE_REPO:-https://github.com/NikitaZotov/scp-machine.git}"
export SCP_MACHINE_BRANCH="${SCP_MACHINE_BRANCH:-feat/move_common_scripts}"
export SCP_MACHINE_COMMIT="${SCP_MACHINE_COMMIT:-""}"

export SC_COMPONENT_MANAGER_REPO="${SC_COMPONENT_MANAGER_REPO:-https://github.com/NikitaZotov/sc-component-manager.git}"
export SC_COMPONENT_MANAGER_BRANCH="${SC_COMPONENT_MANAGER_BRANCH:-feat/move_common_scripts}"
export SC_COMPONENT_MANAGER_COMMIT="${SC_COMPONENT_MANAGER_COMMIT:-""}"

export SC_WEB_REPO="${SC_WEB_REPO:-https://github.com/ostis-ai/sc-web.git}"
export SC_WEB_BRANCH="${SC_WEB_BRANCH:-feature/add-scg-iframe}"
export SC_WEB_COMMIT="${SC_WEB_COMMIT:-""}"

export IMS_KB_REPO="${IMS_KB_REPO:-https://github.com/ostis-ai/ims.ostis.kb.git}"
export IMS_KB_BRANCH="${IMS_KB_BRANCH:-main}"
export IMS_KB_COMMIT="${IMS_KB_COMMIT:-""}"

export STANDARD_REPO="${STANDARD_REPO:-https://github.com/ostis-ai/ostis-standard.git}"
export STANDARD_BRANCH="${STANDARD_BRANCH:-main}"
export STANDARD_COMMIT="${STANDARD_COMMIT:-""}"
export STANDARD_PATH="${STANDARD_PATH:-${METASYSTEM_PATH}/sc-models/kb/ostis-standard}"

export REACT_SC_WEB_REPO="${REACT_SC_WEB_REPO:-https://github.com/ostis-ai/react-sc-web}"
export REACT_SC_WEB_BRANCH="${REACT_SC_WEB_BRANCH:-main}"
export REACT_SC_WEB_COMMIT="${REACT_SC_WEB_COMMIT:-""}"
export REACT_SC_WEB_PATH="${REACT_SC_WEB_PATH:-${METASYSTEM_PATH}/platform-dependent-components/interface/react-sc-web}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
