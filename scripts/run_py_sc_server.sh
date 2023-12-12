#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

python3 "$METASYSTEM_PATH"/platform-dependent-components/problem-solver/py/server.py "$@"
