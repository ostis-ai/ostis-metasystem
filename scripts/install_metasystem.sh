#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"

cd "${CURRENT_DIR}/.." && git submodule update --init --recursive

if [[ -z "${PLATFORM_PATH}" || -z "${METASYSTEM_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

stage "Install OSTIS Metasystem"

"${PLATFORM_PATH}/scripts/install_platform.sh"
source "${CURRENT_DIR}/install_py_sc_server_deps.sh"

"${CURRENT_DIR}/build_problem_solver.sh"
"${CURRENT_DIR}/build_kb.sh"
"${CURRENT_DIR}/build_sc_web.sh"

stage "OSTIS Metasystem is installed successfully"
