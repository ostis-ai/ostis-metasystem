# Quick Start

!!! Note
    Currently, using the OSTIS Metasystem natively on Windows isn't supported.

1. Clone repository of the OSTIS Metasystem:
   
    ```sh
    git clone https://github.com/NikitaZotov/ostis-metasystem
    cd ostis-metasystem
    git checkout feat/update_build_system
    ```

2. Install problem solver of the OSTIS Metasystem:

    ```sh
    ./scripts/install_problem_solver.sh
    ```

3. Build knowledge base:

    ```sh
    ./install/sc-machine/bin/sc-builder -i repo.path -o kb.bin --clear
    ```

4. Run problem solver:

    ```sh
    ./install/sc-machine/bin/sc-machine -s kb.bin -c ostis-metasystem.ini \
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

7. Build and run sc-web. Open new terminal and run:

   ```sh
   cd platform-dependent-components/interface/sc-web
   npm run build
   source .venv/bin/activate && python3 server/app.py --allowed_origins=http://localhost:3000
   cd ..
   ```

8. Install and build interface of the OSTIS Metasystem:

    ```sh
    cd react-sc-web
    npm install
    npm run build
    ```

9. Run interface of the OSTIS Metasystem:

    ```sh
    npm run start
    ```
