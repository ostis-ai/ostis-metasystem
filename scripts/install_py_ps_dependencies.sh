#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

if [ -z "${METASYSTEM_PATH}" ];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Install py problem-solver dependencies"

pip3 install -r "${METASYSTEM_PATH}/platform-dependent-components/problem-solver/py/requirements.txt"

info "Dependencies of py problem-solver is installed successfully"
