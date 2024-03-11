#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"

if [[ -z "${METASYSTEM_CMAKE_PATH}" || -z "${METASYSTEM_CXX_SOURCES_PATH}"
  || -z "${METASYSTEM_BUILD_PATH}" || -z "${BINARY_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

stage "Build CXX problem solver"

BUILD_SCRIPTS="${SC_MACHINE_PATH}/scripts/build-scripts"
"${BUILD_SCRIPTS}/build_cxx_project.sh" -p "${METASYSTEM_CMAKE_PATH}" -s "${METASYSTEM_CXX_SOURCES_PATH}" \
  -b "${METASYSTEM_BUILD_PATH}" --cmake-arg "-DSC_BIN_PATH=${BINARY_PATH}" "$@"

stage "CXX problem solver is built successfully"
