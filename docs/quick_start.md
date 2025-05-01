# Quick Start

!!! Note
    Currently, using the OSTIS Metasystem natively on Windows isn't supported.

1. Clone repository of the OSTIS Metasystem:
   
    ```sh
    git clone https://github.com/ostis-ai/ostis-metasystem
    cd ostis-metasystem
    git submodule update --init --recursive
    ```

2. Install C++ problem solver:

    ```sh
    ./scripts/install_cxx_problem_solver.sh
    ```

3. Build knowledge base:

    ```sh
    ./install/sc-machine/bin/sc-builder -i repo.path -o kb.bin --clear
    ```

4. Run C++ problem solver:

    ```sh
    LD_LIBRARY_PATH=./install/sc-machine/lib ./install/sc-machine/bin/sc-machine -s kb.bin -c ostis-metasystem.ini \
        -e "install/sc-machine/lib/extensions;install/sc-component-manager/lib/extensions;install/scp-machine/lib/extensions;install/problem-solver/lib/extensions"
    ```

5. Download component specifications from repositories. Execute the following command in a running terminal:

    ```sh
    components init
    ```

6. Install sc-web component. Execute the following command in the running terminal:

    ```sh
    components install sc_web
    ```

7. Build sc-web. Open new terminal and run:

    ```sh
    cd platform-dependent-components/interface/install/sc-web
    git checkout feature/add-scg-iframe
    npm run build
    ```

8. Run sc-web:
   
    ```sh
    source .venv/bin/activate && python3 server/app.py --allowed_origins=http://localhost:3000
    ```

9.  Install Python problem solver dependencies. Open new terminal and run:

    ```sh
    python3 -m venv platform-dependent-components/problem-solver/py/.venv
    source platform-dependent-components/problem-solver/py/.venv/bin/activate
    pip3 install -r platform-dependent-components/problem-solver/py/requirements.txt
    ```

10. Run Python problem-solver:
    
    ```sh
    python3 platform-dependent-components/problem-solver/py/server.py
    ```

11. Install and build react interface. Open new terminal and run:

    ```sh
    cd platform-dependent-components/interface/react-sc-web
    npm install
    npm run build
    ```

12. Run react interface:

    ```sh
    npm run start
    ```
