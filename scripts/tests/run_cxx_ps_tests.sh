#!/usr/bin/env bash
set -eo pipefail

if [[ -z "${METASYSTEM_BUILD_PATH}" ]];
then
  source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/../set_vars.sh"
fi

(cd "${METASYSTEM_BUILD_PATH}" && ctest -C Debug -V -R 'starter' --timeout 30)
