# Sample Enclave Programs for the ARMv8 Architecture

This directory contains the unit tests and evaluation case studies for the Pandora port to the ARM Cortex-M TEE for embedded 32-bit microcontrollers. The `ArmCortexM` enclave loader are being integrated into the [upstream Pandora repository](https://github.com/pandora-tee/pandora).

<!-- TODO: More details can be found in Tijs Bellefroids [Msc thesis](...) and our [SysTEX'25 paper](...): -->

<!-- ```bibtex
@inproceedings{goossens2025principled,
    title     = {Principled Symbolic Validation of Enclaves on Low-End Microcontrollers},
    author    = {Goossens, Gert-Jan and Van Bulck, Jo},
    booktitle = {8th Workshop on System Software for Trusted Execution {(SysTEX)}},
    publisher = {{IEEE}},
    month     = Jun,
    year      = 2025,
}
``` -->

## Directory Structure

- **cubemx:** Hand-crafted assembly tests for pointer and control-flow sanitization vulnerabilities

## Reproduction

### Dependencies

Detailed instructions for installing Pandora and all its dependencies in a Python virtual environment are provided in the [Pandora repository](https://github.com/pandora-tee/pandora).

### Symbolic Validation

Support for symbolically validating Sancus enclaves has been fully upstreamed in the [Pandora repository](https://github.com/pandora-tee/pandora).

All directories contain, the source code and compiled binary (`Secure/build/<project_name>_S.elf`).

Proceed as follows to run Pandora on the example enclaves yourself:

```bash
# clone upstream repo
$ git clone https://github.com/pandora-tee/pandora && cd pandora

# create Python virtual environment
$ sudo python3 -m venv venv && source ./venv/bin/activate

# install dependencies in virtual environment
(venv) $ python3 -m pip install -r requirements.txt

# now run pandora on a testcase
(venv) $ ./pandora.py run -p ptr,cf --pandora-option PANDORA_EXPLORE_STACK_DEPTH=1500 -c config-ci.ini /path/to/pandora-examples/armv8/cubemx/Secure/build/cubemx_S.elf --idau-json-file assets/stm32l5.json5 -n 100

# or open a shell on critical ptr events
(venv) $ ./pandora.py run -p ptr,cf --pandora-option PANDORA_EXPLORE_STACK_DEPTH=1500 -c config-ci.ini /path/to/pandora-examples/armv8/cubemx/Secure/build/cubemx_S.elf --idau-json-file assets/stm32l5.json5 -n 100 --action ptr[critical]=shell
```

After Pandora successfully finished, you should see a summary in the console and human-readable HTML reports.

### Compiling and Running

> :warning: This step is _not_ needed to reproduce the results, as we provide precompiled binaries in this repository.

If desired, the test cases can be compiled using the [ARM GCC compiler](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
You can then use the scripts `config.sh`, `compile.sh` to compile the project.

To run the binary on an STM32L5 board (tested on NUCLEO-L552ZE-Q), you need the [STM32_Programmer_CLI](https://www.st.com/en/development-tools/stm32cubeprog.html).
You can then use the provided `deploy.sh` script to flash the board.

