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

export SC_MACHINE_REPO="${SC_MACHINE_REPO:-https://github.com/NikitaZotov/sc-machine.git}"
export SC_MACHINE_BRANCH="${SC_MACHINE_BRANCH:-fix/cmake_deps}"
export SC_MACHINE_COMMIT="${SC_MACHINE_COMMIT:-""}"
export SC_MACHINE_PATH="${SC_MACHINE_PATH:-${PLATFORM_PATH}/sc-machine}"

export SCP_MACHINE_REPO="${SCP_MACHINE_REPO:-https://github.com/NikitaZotov/scp-machine.git}"
export SCP_MACHINE_BRANCH="${SCP_MACHINE_BRANCH:-fix/cmake_deps}"
export SCP_MACHINE_COMMIT="${SCP_MACHINE_COMMIT:-""}"
export SCP_MACHINE_PATH="${SCP_MACHINE_PATH:-${PLATFORM_PATH}/scp-machine}"

export SC_COMPONENT_MANAGER_REPO="${SC_COMPONENT_MANAGER_REPO:-https://github.com/NikitaZotov/sc-component-manager.git}"
export SC_COMPONENT_MANAGER_BRANCH="${SC_COMPONENT_MANAGER_BRANCH:-fix/cmake_deps}"
export SC_COMPONENT_MANAGER_COMMIT="${SC_COMPONENT_MANAGER_COMMIT:-""}"
export SC_COMPONENT_MANAGER_PATH="${SC_COMPONENT_MANAGER_PATH:-${PLATFORM_PATH}/sc-component-manager}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
