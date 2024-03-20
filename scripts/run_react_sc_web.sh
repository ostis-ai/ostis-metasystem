#!/usr/bin/env bash
set -eo pipefail

if [ -z "${REACT_SC_WEB_PATH}" ];
then
  source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"/set_vars.sh
fi

"${REACT_SC_WEB_PATH}/scripts/run_react_sc_web.sh"
