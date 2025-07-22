#!/usr/bin/env bash
set -eo pipefail

# Constants
SC_MACHINE_VERSION="0.10.2"
SC_MACHINE_DESTINATION_DIR="install/sc-machine"

SC_COMPONENT_MANAGER_VERSION="0.2.0"
SC_COMPONENT_MANAGER_DESTINATION_DIR="install/sc-component-manager"

SCP_MACHINE_VERSION="0.1.0"
SCP_MACHINE_DESTINATION_DIR="install/scp-machine"

OSTIS_METASYSTEM_VERSION="0.2.0"
OSTIS_METASYSTEM_DESTINATION_DIR="install/problem-solver"

PS_COMMON_LIB_VERSION="0.1.0"
PS_COMMON_LIB_DESTINATION_DIR="install/ps-common-lib"

NON_ATOMIC_ACTION_INTERPRETER_MODULE_VERSION="0.1.0"
NON_ATOMIC_ACTION_INTERPRETER_MODULE_DESTINATION_DIR="install/non-atomic-action-interpreter-module"

get_archive_name() {
    local os_name=$(uname -s)
    case "$os_name" in
        Linux)
            echo "$1-$2-Linux.tar.gz"
            ;;
        Darwin)
            echo "$1-$2-Darwin.tar.gz"
            ;;
        *)
            echo "Unsupported operating system: $os_name"
            exit 1
            ;;
    esac
}

download_archive() {
    local url="$1"
    echo "Downloading ${url}..."
    if ! curl -LO "${url}"; then
        echo "Error downloading ${url}"
        exit 1
    fi
}

extract_archive() {
    local archive="$1"
    local destination_dir="$2"
    echo "Creating directory ${destination_dir} and extracting files..."
    mkdir -p "${destination_dir}" && tar -xvzf "${archive}" -C "${destination_dir}" --strip-components 1
}

cleanup() {
    local archive="$1"
    local destination_dir="$2"
    echo "Cleaning up..."
    rm -f "${archive}"
    rm -rf "${destination_dir}/include"
}

SC_MACHINE_ARCHIVE=$(get_archive_name "sc-machine" "${SC_MACHINE_VERSION}")
SC_MACHINE_URL="https://github.com/ostis-ai/sc-machine/releases/download/${SC_MACHINE_VERSION}/${SC_MACHINE_ARCHIVE}"

download_archive "${SC_MACHINE_URL}"
extract_archive "${SC_MACHINE_ARCHIVE}" "${SC_MACHINE_DESTINATION_DIR}"
cleanup "${SC_MACHINE_ARCHIVE}" "${SC_MACHINE_DESTINATION_DIR}"

SC_COMPONENT_MANAGER_ARCHIVE=$(get_archive_name "sc-component-manager" "${SC_COMPONENT_MANAGER_VERSION}")
SC_COMPONENT_MANAGER_URL="https://github.com/ostis-ai/sc-component-manager/releases/download/${SC_COMPONENT_MANAGER_VERSION}/${SC_COMPONENT_MANAGER_ARCHIVE}"

download_archive "${SC_COMPONENT_MANAGER_URL}"
extract_archive "${SC_COMPONENT_MANAGER_ARCHIVE}" "${SC_COMPONENT_MANAGER_DESTINATION_DIR}"
cleanup "${SC_COMPONENT_MANAGER_ARCHIVE}" "${SC_COMPONENT_MANAGER_DESTINATION_DIR}"

SCP_MACHINE_ARCHIVE=$(get_archive_name "scp-machine" "${SCP_MACHINE_VERSION}")
SCP_MACHINE_URL="https://github.com/ostis-ai/scp-machine/releases/download/${SCP_MACHINE_VERSION}/${SCP_MACHINE_ARCHIVE}"

download_archive "${SCP_MACHINE_URL}"
extract_archive "${SCP_MACHINE_ARCHIVE}" "${SCP_MACHINE_DESTINATION_DIR}"
cleanup "${SCP_MACHINE_ARCHIVE}" "${SCP_MACHINE_DESTINATION_DIR}"

OSTIS_METASYSTEM_ARCHIVE=$(get_archive_name "ostis-metasystem" "${OSTIS_METASYSTEM_VERSION}")
OSTIS_METASYSTEM_URL="https://github.com/ostis-ai/ostis-metasystem/releases/download/${OSTIS_METASYSTEM_VERSION}/${OSTIS_METASYSTEM_ARCHIVE}"

download_archive "${OSTIS_METASYSTEM_URL}"
extract_archive "${OSTIS_METASYSTEM_ARCHIVE}" "${OSTIS_METASYSTEM_DESTINATION_DIR}"
cleanup "${OSTIS_METASYSTEM_ARCHIVE}" "${OSTIS_METASYSTEM_DESTINATION_DIR}"

PS_COMMON_LIB_ARCHIVE=$(get_archive_name "ps-common-lib" "${PS_COMMON_LIB_VERSION}")
PS_COMMON_LIB_URL="https://github.com/ostis-ai/ostis-ps-lib/releases/download/${PS_COMMON_LIB_VERSION}/${PS_COMMON_LIB_ARCHIVE}"

download_archive "${PS_COMMON_LIB_URL}"
extract_archive "${PS_COMMON_LIB_ARCHIVE}" "${PS_COMMON_LIB_DESTINATION_DIR}"
cleanup "${PS_COMMON_LIB_ARCHIVE}" "${PS_COMMON_LIB_DESTINATION_DIR}"

NON_ATOMIC_ACTION_INTERPRETER_MODULE_ARCHIVE=$(get_archive_name "non-atomic-action-interpreter-module" "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_VERSION}")
NON_ATOMIC_ACTION_INTERPRETER_MODULE_URL="https://github.com/ostis-ai/ostis-ps-lib/releases/download/${NON_ATOMIC_ACTION_INTERPRETER_MODULE_VERSION}/${NON_ATOMIC_ACTION_INTERPRETER_MODULE_ARCHIVE}"

download_archive "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_URL}"
extract_archive "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_ARCHIVE}" "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_DESTINATION_DIR}"
cleanup "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_ARCHIVE}" "${NON_ATOMIC_ACTION_INTERPRETER_MODULE_DESTINATION_DIR}"

echo "Installation of ostis-metasystem complete!"
