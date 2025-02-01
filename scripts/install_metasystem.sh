#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"

METASYSTEM_PATH="${CURRENT_DIR}/.."
cd "${METASYSTEM_PATH}" && git submodule update --init --recursive

source "${CURRENT_DIR}/set_vars.sh"

stage "Install ostis-metasystem"

"${CURRENT_DIR}/install_react_sc_web.sh"
"${CURRENT_DIR}/install_py_ps_deps.sh"
"${CURRENT_DIR}/build_ps.sh"
"${CURRENT_DIR}/build_kb.sh"
"${CURRENT_DIR}/build_ui.sh"

stage "ostis-metasystem is installed successfully"
