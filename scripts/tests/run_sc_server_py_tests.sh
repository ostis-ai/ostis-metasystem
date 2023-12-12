#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/../set_vars.sh"

"$METASYSTEM_PATH"/scripts/run_sc_server.sh --repo_path "${APP_ROOT_PATH}"/kb.bin-tests --clear -p 8091 &
