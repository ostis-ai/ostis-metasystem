#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/../set_vars.sh"

if ( ! python3 "${PLATFORM_PATH}/sc-machine/scripts/healthcheck.py" --port 8091 )
then
    echo "sc-server for python tests is not running. Execute ./run_sc_server_py_tests.sh"
    exit 1
fi

python3 -m unittest discover "$METASYSTEM_PATH"/platform-dependent-components/problem-solver/py/
