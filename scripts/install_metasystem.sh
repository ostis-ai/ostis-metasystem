#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"

if [[ -z "${PLATFORM_PATH}" || -z "${METASYSTEM_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

stage "Install OSTIS Metasystem"

cd "${METASYSTEM_PATH}" && git submodule update --init --recursive
"${PLATFORM_PATH}/scripts/install_platform.sh"

stage "OSTIS Metasystem installed successfully"
