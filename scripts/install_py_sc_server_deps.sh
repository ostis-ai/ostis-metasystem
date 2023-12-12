#!/usr/bin/env bash
source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

pip3 install -r "$METASYSTEM_PATH/platform-dependent-components/problem-solver/py/requirements.txt"
