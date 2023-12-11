#!/usr/bin/env bash
set -eo pipefail

ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export APP_ROOT_PATH="${APP_ROOT_PATH:-${ROOT_PATH}}"
export METASYSTEM_PATH="${METASYSTEM_PATH:-${APP_ROOT_PATH}}"
export PLATFORM_PATH="${PLATFORM_PATH:-${APP_ROOT_PATH}/ostis-web-platform}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
