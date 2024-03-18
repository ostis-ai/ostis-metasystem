#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)

if [[ -z "${REACT_SC_WEB_REPO}" || -z "${REACT_SC_WEB_PATH}" || -z "${REACT_SC_WEB_BRANCH}" || -z "${REACT_SC_WEB_COMMIT}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

UTILS_DIR="${PLATFORM_PATH}/scripts/submodule-scripts/utils"
"${UTILS_DIR}/install_submodule.sh" --repo "${REACT_SC_WEB_REPO}" --path "${REACT_SC_WEB_PATH}" --branch "${REACT_SC_WEB_BRANCH}" --commit "${REACT_SC_WEB_COMMIT}"

"${REACT_SC_WEB_PATH}/scripts/install_react_sc_web.sh"
