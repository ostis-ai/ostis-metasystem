#!/usr/bin/env bash
set -eo pipefail

ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export METASYSTEM_PATH="${METASYSTEM_PATH:-${ROOT_PATH}}"

export METASYSTEM_CMAKE_PATH="${METASYSTEM_CMAKE_PATH:-${ROOT_PATH}}"
export METASYSTEM_CXX_SOURCES_PATH="${METASYSTEM_PATH}/platform-dependent-components/problem-solver"
export METASYSTEM_PY_SOURCES_PATH="${METASYSTEM_PATH}/platform-dependent-components/problem-solver/py"
export METASYSTEM_BUILD_PATH="${METASYSTEM_BUILD_PATH:-${METASYSTEM_CMAKE_PATH}/build}"
export BINARY_PATH="${BINARY_PATH:-${METASYSTEM_CMAKE_PATH}/bin}"

export CONFIG_PATH="${CONFIG_PATH:-${METASYSTEM_PATH}/ostis-metasystem.ini}"
export REPO_PATH_FILE="${REPO_PATH_FILE:-${METASYSTEM_PATH}/repo.path}"

export PLATFORM_PATH="${PLATFORM_PATH:-${METASYSTEM_PATH}/ostis-web-platform}"

export SC_WEB_REPO="${SC_WEB_REPO:-https://github.com/ostis-ai/sc-web.git}"
export SC_WEB_BRANCH="${SC_WEB_BRANCH:-main}"
export SC_WEB_COMMIT="${SC_WEB_COMMIT:-"527a7a728e7d547f480dd106763c1d66a625fad6"}"

export SC_MACHINE_REPO="${SC_MACHINE_REPO:-https://github.com/ostis-ai/sc-machine.git}"
export SC_MACHINE_BRANCH="${SC_MACHINE_BRANCH:-main}"
export SC_MACHINE_COMMIT="${SC_MACHINE_COMMIT:-"d9b4ef25e39ece566d1b5f0931b8d2f3354542ad"}"

export REACT_SC_WEB_REPO="${REACT_SC_WEB_REPO:-https://github.com/ostis-ai/react-sc-web}"
export REACT_SC_WEB_PATH="${REACT_SC_WEB_PATH:-${METASYSTEM_PATH}/platform-dependent-components/interface/react-sc-web}"
export REACT_SC_WEB_BRANCH="${REACT_SC_WEB_BRANCH:-main}"
export REACT_SC_WEB_COMMIT="${REACT_SC_WEB_COMMIT:-""}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
