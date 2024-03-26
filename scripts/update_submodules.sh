#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

SUBMODULE_SCRIPTS_DIR="${SCRIPTS_DIR}/submodule-scripts"

if [[ -z "${METASYSTEM_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Update submodules"

cd "${METASYSTEM_PATH}" && git submodule update --init --recursive

"${SUBMODULE_SCRIPTS_DIR}/update_platform.sh"
"${SUBMODULE_SCRIPTS_DIR}/update_standard.sh"

info "Submodules updated successfully"
