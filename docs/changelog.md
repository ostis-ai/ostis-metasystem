# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 09.03.2025

### Added
- Sections module
- ClangFormat for code formatting
- Base structure of the Metasystem
- sc-component-manager paths for components
- Script `sc-component-manager.sh`
- Added build of sc-machine and manager in `install_metasystem.sh`

### Changed
- This version of the Metasystem is not supported on macOS.
- Rename questions to actions
- Use `build_platform.sh` instead of `build_sc_machine.sh`
- Update ostis-web-platform to develop-branch

### Fixed 
- Replaced deprecated options `save_period` and `update_period`

### Deprecated

### Removed
- Remove codegen for agents
