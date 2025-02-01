# ostis-metasystem

[![CI](https://github.com/ostis-ai/ostis-metasystem/actions/workflows/test_conan.yml/badge.svg)](https://github.com/ostis-ai/ostis-metasystem/actions/workflows/test_conan.yml)
[![license](https://img.shields.io/badge/License-MIT-yellow.svg)](COPYING.MIT)

[![Quick Start](https://img.shields.io/badge/-Quick%20Start-black?style=for-the-badge&logo=rocket)](https://ostis-ai.github.io/ostis-metasystem/quick_start)
[![Docs](https://img.shields.io/badge/Docs-gray?style=for-the-badge&logo=read-the-docs)](https://ostis-ai.github.io/ostis-metasystem)
[![Community](https://img.shields.io/badge/-Community-teal?style=for-the-badge&logo=matrix)](https://app.element.io/index.html#/room/#ostis_tech_support:matrix.org)

This repository is a storage of the Metasystem of the [OSTIS Technology](https://github.com/ostis-ai).

OSTIS Metasystem contains:
1. OSTIS Metasystem knowledge base;
2. OSTIS Metasystem problem solver;
3. OSTIS Metasystem user interface.

To learn more about the OSTIS Metasystem, check out our [documentation](https://libeldoc.bsuir.by/handle/123456789/51151).

## Getting started

To get started, check out our [quick start guide](https://ostis-ai.github.io/ostis-metasystem/quick_start).

## Documentation

- A brief user manual and developer docs are hosted on our [GitHub Pages](https://ostis-ai.github.io/ostis-metasystem).
  - <details>
      <summary>Build documentation locally</summary>

    ```sh
    pip3 install mkdocs mkdocs-material
    mkdocs serve
    # and open http://127.0.0.1:8003/ in your browser
    ```
    </details>

- Documentation about implementation of components is located in [docs/main.pdf](docs/main.pdf) file of this project.
  - <details>
       <summary>Build documentation locally</summary>
      
    - ### Build steps (using LaTeX)
      ```sh
      cd docs
      TEXINPUTS=./scn: latexmk -pdf -bibtex main.tex
      ```

    - ### Build steps (using Docker)

      ```sh
      docker run -v ${PWD}:/workdir --rm -it ostis/scn-latex-plugin:latest "docs/main.tex"
      ```

    - ### Download scn-tex-plugin and documentation for subprojects

      After the compilation, the `main.pdf` file should appear at `ostis-metasystem/docs/`. You can find more information about [scn-latex-plugin here](https://github.com/ostis-ai/scn-latex-plugin).
    </details>

## Feedback

Contributions, bug reports and feature requests are welcome! Feel free to check our [issues page](https://github.com/ostis-ai/ostis-metasystem/issues) and file a new issue (or comment in existing ones).

## License

Distributed under the MIT License. Check [COPYING.MIT](COPYING.MIT) for more information.
