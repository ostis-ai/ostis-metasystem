#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

info "Install ostis-metasystem"

"${CURRENT_DIR}/install_minimal_submodules.sh"
"${CURRENT_DIR}/install_dependencies.sh"
"${CURRENT_DIR}/build_platform.sh"
"${CURRENT_DIR}/build_ps.sh"
"${CURRENT_DIR}/build_kb.sh"
"${CURRENT_DIR}/build_ui.sh"

info "ostis-metasystem is installed successfully"
