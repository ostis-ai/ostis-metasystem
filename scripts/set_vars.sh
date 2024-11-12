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

export PLATFORM_PATH="${PLATFORM_PATH:-${METASYSTEM_PATH}/ostis-web-platform}"

export SC_WEB_REPO="${SC_WEB_REPO:-https://github.com/NikitaZotov/sc-web.git}"
export SC_WEB_BRANCH="${SC_WEB_BRANCH:-feat/harmonise_sc_types}"
export SC_WEB_COMMIT="${SC_WEB_COMMIT:-""}"

export SC_MACHINE_REPO="${SC_MACHINE_REPO:-https://github.com/NikitaZotov/sc-machine.git}"
export SC_MACHINE_BRANCH="${SC_MACHINE_BRANCH:-fix/kb_builder_config}"
export SC_MACHINE_COMMIT="${SC_MACHINE_COMMIT:-""}"

export REACT_SC_WEB_REPO="${REACT_SC_WEB_REPO:-https://github.com/ostis-ai/react-sc-web}"
export REACT_SC_WEB_PATH="${REACT_SC_WEB_PATH:-${METASYSTEM_PATH}/platform-dependent-components/interface/react-sc-web}"
export REACT_SC_WEB_BRANCH="${REACT_SC_WEB_BRANCH:-main}"
export REACT_SC_WEB_COMMIT="${REACT_SC_WEB_COMMIT:-""}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
