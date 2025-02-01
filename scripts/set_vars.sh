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
export SC_WEB_COMMIT="${SC_WEB_COMMIT:-"17aa0ca9912021119c2a67ba70e355c8dff35082"}"

export REACT_SC_WEB_REPO="${REACT_SC_WEB_REPO:-https://github.com/ostis-ai/react-sc-web}"
export REACT_SC_WEB_PATH="${REACT_SC_WEB_PATH:-${METASYSTEM_PATH}/platform-dependent-components/interface/react-sc-web}"
export REACT_SC_WEB_BRANCH="${REACT_SC_WEB_BRANCH:-main}"
export REACT_SC_WEB_COMMIT="${REACT_SC_WEB_COMMIT:-""}"
