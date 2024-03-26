#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
SUBMODULE_SCRIPTS_DIR="${CURRENT_DIR}/../ostis-scripts/submodule-scripts"

if [[ -z "${PLATFORM_REPO}" || -z "${PLATFORM_PATH}" || -z "${PLATFORM_BRANCH}" || -z "${PLATFORM_COMMIT}" ]];
then
  source "${CURRENT_DIR}/../set_vars.sh"
fi

"${SUBMODULE_SCRIPTS_DIR}/install_submodule.sh" --repo "${PLATFORM_REPO}" --path "${PLATFORM_PATH}" --branch "${PLATFORM_BRANCH}" --commit "${PLATFORM_COMMIT}"
"${PLATFORM_PATH}/scripts/install_submodules.sh"
