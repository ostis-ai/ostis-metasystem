#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

if [ -z "${REACT_SC_WEB_PATH}" ];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Install react sc-web dependencies"

"${REACT_SC_WEB_PATH}/scripts/install_react_sc_web_dependencies.sh" "$@"

info "Dependencies of react sc-web is installed successfully"
