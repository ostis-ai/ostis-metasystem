#!/usr/bin/env bash
source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

python3 -m venv "${METASYSTEM_PY_SOURCES_PATH}/.venv"
source "${METASYSTEM_PY_SOURCES_PATH}/.venv/bin/activate"
pip3 install -r "${METASYSTEM_PY_SOURCES_PATH}/requirements.txt"
