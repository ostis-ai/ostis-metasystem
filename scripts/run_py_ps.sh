#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

source "${METASYSTEM_PY_SOURCES_PATH}/.venv/bin/activate"
python3 "${METASYSTEM_PY_SOURCES_PATH}/server.py" "$@"
