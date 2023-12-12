# ostis-metasystem

[![license](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This repository is a metasystem of the [OSTIS Project](https://github.com/ostis-ai/ostis-project).

ostis-metasystem contains:

1. [ostis-web-platfrom](https://github.com/ostis-ai/ostis-web-platfrom) to interpret sc-models
2. sc-models of the Metasystem
   1. Metasystem knowledge base
   2. Metasystem problem solver
   3. Metasystem user interface

To learn more about the Metasystem, check out our [documentation](https://libeldoc.bsuir.by/handle/123456789/51151).

## Installation

- Natively

  Note: Currently, only Linux (Ubuntu-20.04, Ubuntu-22.04) and macOS are supported by this installation method. If you're going to use it, it might take a while to download dependencies and compile the components. Use it only if you know what you're doing!

  ```sh
  git clone https://github.com/ostis-ai/ostis-metasystem
  ./scripts/install_metasystem.sh
  ```

## Usage

- Native

  ```sh
  # Launch knowledge processing machine
  ./scripts/run_sc_server.sh
  # *in another terminal*
  # Launch semantic web interface at localhost:8000
  ./scripts/run_sc_web.sh
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
