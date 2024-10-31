# ask-ai
The system can answer 2 types of questions:

1. about entity
2. about subdividing

What can be improved:

1. make ask-ai multilingual, now it is russian 
2. make llm promt better (sometimes its answers are bad)
3. add more types of questions

# ostis-metasystem

[![license](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This repository is a storage of the Metasystem of the [OSTIS Project](https://github.com/ostis-ai/ostis-project).

OSTIS Metasystem contains:

1. [OSTIS Platform](https://github.com/ostis-ai/ostis-web-platfrom) to interpret sc-models
2. sc-models of the OSTIS Metasystem
   1. OSTIS Metasystem knowledge base
   2. OSTIS Metasystem problem solver
   3. OSTIS Metasystem user interface

To learn more about the OSTIS Metasystem, check out our [documentation](https://libeldoc.bsuir.by/handle/123456789/51151).

## Installation

- Natively

  Note: Currently, only Linux (Ubuntu-20.04, Ubuntu-22.04) and macOS are supported by this installation method. If you're going to use it, it might take a while to download dependencies and compile the components. Use it only if you know what you're doing!

  ```sh
  git clone https://github.com/ostis-ai/ostis-metasystem
  cd ostis-metasystem
  git checkout demo
  git submodule update --init --recursive
  ./scripts/install_metasystem.sh
  ```

## Usage

- Native

  ```sh
  # Launch cxx problem solver
  ./scripts/run_cxx_ps.sh
  # *in another terminal*
  # Launch sc-web user interface at localhost:8000
  ./scripts/run_sc_web.sh
  # *in another terminal*
  # Launch react-sc-web user interface at localhost:3000
  ./scripts/run_react_sc_web.sh
  ```

## Documentation

We document all information about the project development and its components' implementation in sources of its knowledge base
to provide opportunity to use it in information processing and knowledge generation.

You can access the current version of the documentation in [docs/main.pdf](docs/main.pdf) file of this project.

Documentation is written with the help of LaTeX tools in SCn-code representation. To build documentation manually, 
you'll need a LaTeX distribution installed on your computer. 
Alternatively, we provide a Docker image to build the documentation in case you can't / don't want to install LaTeX on your PC.

### Download scn-tex-plugin and documentation for subprojects

- ### Build steps (using LaTeX)

  ```sh
  cd docs
  TEXINPUTS=./scn: latexmk -pdf -bibtex main.tex
  ```

- ### Build steps (using Docker)

  ```sh
  docker run -v ${PWD}:/workdir --rm -it ostis/scn-latex-plugin:latest "docs/main.tex"
  ```

  After the compilation, the `main.pdf` file should appear at `ostis-metasystem/docs/`. You can find more information about [scn-latex-plugin here](https://github.com/ostis-ai/scn-latex-plugin).

## Feedback

Contributions, bug reports and feature requests are welcome! Feel free to check our [issues page](https://github.com/ostis-ai/ostis-metasystem/issues) and file a new issue (or comment in existing ones).

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.
