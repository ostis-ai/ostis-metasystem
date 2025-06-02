# Quick Start for Contributors

This guide provides short information for developers to start to work with ostis-metasystem quickly. You can always learn more about the ostis-metasystem's [build system](build_system.md).

## Install project

Install ostis-metasystem via git:

```sh
git clone https://github.com/ostis-ai/ostis-metasystem
cd ostis-metasystem
git submodule update --init --recursive
```

## Check CMake

Install pipx first using [**pipx installation guide**](https://pipx.pypa.io/stable/installation/) if not already installed.

Ensure you are using **CMake version 3.24** or newer. Verify your version with:

```sh
cmake --version
```

To upgrade CMake, run:
  
```sh
# Use pipx to install cmake if not already installed
pipx install cmake
pipx ensurepath
# relaunch your shell after installation
exec $SHELL
```

Install Ninja generator for CMake, to use sc-machine CMake presets:

```sh
# Use pipx to install ninja if not already installed
pipx install ninja
pipx ensurepath
# relaunch your shell after installation
exec $SHELL
```

## Start develop C++ problem solver of ostis-metasystem with Conan

### Install Conan

Install Conan, to build C++ problem solver dependencies with Conan-provided dependencies:

```sh
# Use pipx to install conan if not already installed
pipx install conan
pipx ensurepath
# relaunch your shell after installation
exec $SHELL
```

### Add Conan remote repository

Add Conan remote repository with the specified URL:

```sh
conan remote add ostis-ai https://conan.ostis.net/artifactory/api/conan/ostis-ai-library
```

### Use C++ problem solver in Debug

#### Install dependencies with Conan

C++ problem solver of ostis-metasystem is an extension to sc-machine, so sc-machine is main dependency for C++ problem solver. To install it, run in the root of the project:

```sh
conan install . -s build_type=Debug --build=missing
```

#### Build C++ problem solver in Debug

To build C++ problem solver in debug mode using Conan-provided dependencies, run:

```sh
# debug build type
cmake --preset debug-conan
cmake --build --preset debug
```

!!! Note
    By default, configure preset `debug` enables building C++ problem solver tests.

#### Run C++ problem solver tests in Debug

After that, you can go to `build/Debug` and run tests via `ctest`:

```sh
cd build/Debug
ctest -V
```

You can also check code formatting. To learn more, go to the [CMake flags](cmake_flags.md) page.

### Use C++ problem solver in Release

#### Install dependencies with Conan

To install it, run in the root of the project:

```sh
conan install . -s build_type=Release --build=missing
```

#### Build C++ problem solver in Release

To build C++ problem solver in release mode using Conan-provided dependencies, run:

```sh
# release build type without tests
cmake --preset release-conan
cmake --build --preset release
```

To build C++ problem solver with tests in release mode using Conan-provided dependencies, run:

```sh
# release build type with tests
cmake --preset release-with-tests-conan
cmake --build --preset release
```

#### Run C++ problem solver tests in Release

After that, you can run tests:

```sh
cd build/Release
ctest -V
```

You can also check code formatting. To learn more, go to the [CMake flags](cmake_flags.md) page.

## Start develop Python problem solver of ostis-metasystem

### Install dependencies with pip3

To install dependencies, run:

```sh
python3 -m venv platform-dependent-components/problem-solver/py/.venv
source platform-dependent-components/problem-solver/py/.venv/bin/activate
pip3 install -r platform-dependent-components/problem-solver/py/requirements.txt
```

### Run Python problem solver tests

Running Python problem solver requires C++ problem solver to be running. Run C++ problem solver:

```sh
./path/to/sc-machine/binary -s kb.bin \
    -e "path/to/ostis-metasystem/lib/extensions;path/to/sc-machine/lib/extensions"
```

and after that, run Python problem-solver tests in new terminal:

```sh
source platform-dependent-components/problem-solver/py/.venv/bin/activate && \
python3 -m unittest discover platform-dependent-components/problem-solver/py
```

### Run Python problem solver

Run C++ problem solver:

```sh
./path/to/sc-machine/binary -s kb.bin \
    -e "path/to/ostis-metasystem/lib/extensions;path/to/sc-machine/lib/extensions"
```

and after that, run Python problem-solver in new terminal:

```sh
source platform-dependent-components/problem-solver/py/.venv/bin/activate && \
python3 platform-dependent-components/problem-solver/py/server.py
```

## Start develop interface of ostis-metasystem

### Install dependencies with npm

To install dependencies, go to the `react-sc-web` directory and run:

```sh
cd platform-dependent-components/interface/react-sc-web
npm install
```

### Build interface 

To build interface, run:

```sh
npm run build
```

### Run interface

After run:

```sh
npm run start
```

## Run ostis-metasystem

Before launching ostis-metasystem:

- [extract sc-machine from GitHub Releases](https://ostis-ai.github.io/sc-machine/quick_start/#github-releases) or [build it](https://ostis-ai.github.io/sc-machine/build/quick_start/);
- [extract scp-machine from GitHub Releases](https://ostis-ai.github.io/scp-machine/quick_start/#github-releases) or [build it](https://ostis-ai.github.io/scp-machine/build/quick_start/);
- [extract sc-component-manager from GitHub Releases](https://ostis-ai.github.io/sc-component-manager/quick_start/#github-releases) or [build it](https://ostis-ai.github.io/sc-component-manager/build/quick_start/).
- [extract ps-common-lib from GitHub Releases](https://github.com/ostis-ai/ostis-ps-lib/releases) or [build it](https://ostis-ai.github.io/ostis-ps-lib/ps-common-lib/#installation-build).
- [extract non-atomic-action-interpreter-module from GitHub Releases](https://ostis-ai.github.io/ostis-ps-lib/non-atomic-action-interpreter-module/#using-module-as-an-extension-for-sc-machine) or [build it](https://ostis-ai.github.io/ostis-ps-lib/non-atomic-action-interpreter-module/#installation-build).

Firstly, build knowledge base, make sure `repo.path` file contains paths to `section_subject_domain_of_messages` and `non_subject_domain_concepts` folders of [nika](https://github.com/ostis-apps/nika/):

```sh
./path/to/sc-builder/binary -i repo.path -o kb.bin --clear
``` 

After run C++ problem solver:

```sh
./path/to/sc-machine/binary -s kb.bin -c ostis-metasystem.ini \
    -e "path/to/ostis-metasystem/lib/extensions;path/to/sc-machine/lib/extensions;path/to/scp-machine/lib/extensions;path/to/sc-component-manager/lib/extensions;path/to/non-atomic-action-interpreter-module/lib/extensions"
# if several paths to extensions are provided then they should be separated 
# by semicolon and wrapped in double quotes
```

Run Python problem solver in new terminal:

```sh
source platform-dependent-components/problem-solver/py/.venv/bin/activate && \
python3 platform-dependent-components/problem-solver/py/server.py
```

And run react-sc-web in new terminal:

```sh
cd platform-dependent-components/interface/react-sc-web
npm run start
```

### Contributing

To contribute changes to the project, you need to [create Pull Request](https://github.com/ostis-ai/ostis-metasystem/blob/main/docs/CONTRIBUTING.md).
