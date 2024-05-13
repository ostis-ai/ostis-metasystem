#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

if [ -z "${PLATFORM_PATH}" ];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Install ostis-platform dependencies"

"${PLATFORM_PATH}/scripts/install_dependencies.sh" "$@"

info "Dependencies of ostis-platform is installed successfully"
