#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
SUBMODULE_SCRIPTS_DIR="${CURRENT_DIR}/../ostis-scripts/submodule-scripts"

if [[ -z "${STANDARD_REPO}" || -z "${STANDARD_PATH}" || -z "${STANDARD_BRANCH}" || -z "${STANDARD_COMMIT}" ]];
then
  source "${CURRENT_DIR}/../set_vars.sh"
fi

"${SUBMODULE_SCRIPTS_DIR}/update_submodule.sh" --repo "${STANDARD_REPO}" --path "${STANDARD_PATH}" --branch "${STANDARD_BRANCH}" --commit "${STANDARD_COMMIT}"
