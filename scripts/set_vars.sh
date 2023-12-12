#!/usr/bin/env bash
set -eo pipefail

ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export APP_ROOT_PATH="${APP_ROOT_PATH:-${ROOT_PATH}}"
export ROOT_CMAKE_PATH="${ROOT_CMAKE_PATH:-${APP_ROOT_PATH}}"
export METASYSTEM_PATH="${METASYSTEM_PATH:-${APP_ROOT_PATH}}"
export CONFIG_PATH="${CONFIG_PATH:-${METASYSTEM_PATH}/ostis-metasystem.ini}"
export REPO_PATH_FILE="${REPO_PATH_FILE:-${METASYSTEM_PATH}/repo.path}"
export PLATFORM_PATH="${PLATFORM_PATH:-${METASYSTEM_PATH}/ostis-web-platform}"

if [[ -d "${PLATFORM_PATH}" ]];
then
  source "${PLATFORM_PATH}/scripts/set_vars.sh"
fi
