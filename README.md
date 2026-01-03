# VibeThing

The [Internet of things (IoT)](https://grokipedia.com/page/IOT), [Zigbee](https://grokipedia.com/page/Zigbee), [Matter](https://grokipedia.com/page/Matter_(standard)), [Thread](https://grokipedia.com/page/Thread_(network_protocol)), [Wi-Fi](https://grokipedia.com/page/Wi-Fi), [Bluetooth](https://grokipedia.com/page/Bluetooth), [STM32](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html), [ESP32](https://www.espressif.com/en/products/socs/esp32), [FreeRTOS](https://grokipedia.com/page/FreeRTOS), [SCADA](https://grokipedia.com/page/SCADA), [Arduino](https://grokipedia.com/page/Arduino), [Raspberry Pi](https://grokipedia.com/page/Raspberry_Pi)

---
## 📝 License

Copyright © 2010 — 2026 [Randolph Ledesma](https://gitlab.com/randop).
>
> Licensed under the Apache License, Version 2.0 (the "License");
> you may not use this file except in compliance with the License.
> You may obtain a copy of the License at
>
>    http://www.apache.org/licenses/LICENSE-2.0
>
> Unless required by applicable law or agreed to in writing, software
> distributed under the License is distributed on an "AS IS" BASIS,
> WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
> See the License for the specific language governing permissions and
> limitations under the License.
>

Last updated on 2026-01-01T00:00:00.000Z

---
### Develop

#### Setup
```shell
sudo pacman -S --needed \
    base-devel git pcre2 wget flex bison gperf \
    python python-pip python-virtualenv \
    cmake ninja ccache \
    libffi openssl dfu-util libusb

mkdir -p ~/esp
cd ~/esp
git clone -b v5.5.2 --recursive https://github.com/espressif/esp-idf.git
cd ~/esp/esp-idf
./install.fish esp32c6,esp32
. $HOME/esp/esp-idf/export.fish
alias get_idf='. $HOME/esp/esp-idf/export.sh'

cd ~/esp
git clone --recursive https://github.com/espressif/esp-zigbee-sdk.git
cd esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch
idf.py set-target esp32c6
idf.py erase-flash
idf.py build
idf.py -p /dev/ttyACM0 flash
idf.py -p /dev/ttyACM0 monitor
```

#### Linux dmesg logs
```
[ 3479.769042] usb 3-2: new full-speed USB device number 3 using xhci_hcd
[ 3479.924055] usb 3-2: New USB device found, idVendor=303a, idProduct=1001, bcdDevice= 1.02
[ 3479.924066] usb 3-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 3479.924071] usb 3-2: Product: USB JTAG/serial debug unit
[ 3479.924075] usb 3-2: Manufacturer: Espressif
[ 3479.924079] usb 3-2: SerialNumber: 00:00:00:00:00:00
[ 3479.951188] cdc_acm 3-2:1.0: ttyACM0: USB ACM device
[ 3479.951230] usbcore: registered new interface driver cdc_acm
[ 3479.951233] cdc_acm: USB Abstract Control Model driver for USB modems and ISDN adapters

[ 3539.542300] usb 3-2: USB disconnect, device number 3
[ 3542.577045] usb 3-2: new full-speed USB device number 4 using xhci_hcd
[ 3542.731152] usb 3-2: New USB device found, idVendor=303a, idProduct=1001, bcdDevice= 1.02
[ 3542.731163] usb 3-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 3542.731168] usb 3-2: Product: USB JTAG/serial debug unit
[ 3542.731173] usb 3-2: Manufacturer: Espressif
[ 3542.731177] usb 3-2: SerialNumber: 00:00:00:00:00:00
[ 3542.739226] cdc_acm 3-2:1.0: ttyACM0: USB ACM device
```

---
#### Commands and Logs

```shell
./install.fish esp32c6,esp32
```
```
INFO: Using IDF_PATH '/home/johnpaul/esp/esp-idf' for installation.
Detecting the Python interpreter
Checking "python3" ...
Python 3.13.11
python3 has been detected
Checking Python compatibility
Installing ESP-IDF tools
Selected targets are: esp32, esp32c6
Current system platform: linux-amd64
Installing tools: xtensa-esp-elf-gdb, riscv32-esp-elf-gdb, xtensa-esp-elf, riscv32-esp-elf, esp32ulp-elf, openocd-esp32, esp-rom-elfs
Installing xtensa-esp-elf-gdb@16.3_20250913
Downloading https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v16.3_20250913/xtensa-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz
Destination: /home/johnpaul/.espressif/dist/xtensa-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/xtensa-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz to /home/johnpaul/.espressif/tools/xtensa-esp-elf-gdb/16.3_20250913
Installing riscv32-esp-elf-gdb@16.3_20250913
Downloading https://github.com/espressif/binutils-gdb/releases/download/esp-gdb-v16.3_20250913/riscv32-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz
Destination: /home/johnpaul/.espressif/dist/riscv32-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/riscv32-esp-elf-gdb-16.3_20250913-x86_64-linux-gnu.tar.gz to /home/johnpaul/.espressif/tools/riscv32-esp-elf-gdb/16.3_20250913
Installing xtensa-esp-elf@esp-14.2.0_20251107
Downloading https://github.com/espressif/crosstool-NG/releases/download/esp-14.2.0_20251107/xtensa-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz
Destination: /home/johnpaul/.espressif/dist/xtensa-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/xtensa-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz to /home/johnpaul/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20251107
Installing riscv32-esp-elf@esp-14.2.0_20251107
Downloading https://github.com/espressif/crosstool-NG/releases/download/esp-14.2.0_20251107/riscv32-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz
Destination: /home/johnpaul/.espressif/dist/riscv32-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/riscv32-esp-elf-14.2.0_20251107-x86_64-linux-gnu.tar.xz to /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107
Installing esp32ulp-elf@2.38_20240113
Downloading https://github.com/espressif/binutils-gdb/releases/download/esp32ulp-elf-2.38_20240113/esp32ulp-elf-2.38_20240113-linux-amd64.tar.gz
Destination: /home/johnpaul/.espressif/dist/esp32ulp-elf-2.38_20240113-linux-amd64.tar.gz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/esp32ulp-elf-2.38_20240113-linux-amd64.tar.gz to /home/johnpaul/.espressif/tools/esp32ulp-elf/2.38_20240113
Installing openocd-esp32@v0.12.0-esp32-20250707
Downloading https://github.com/espressif/openocd-esp32/releases/download/v0.12.0-esp32-20250707/openocd-esp32-linux-amd64-0.12.0-esp32-20250707.tar.gz
Destination: /home/johnpaul/.espressif/dist/openocd-esp32-linux-amd64-0.12.0-esp32-20250707.tar.gz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/openocd-esp32-linux-amd64-0.12.0-esp32-20250707.tar.gz to /home/johnpaul/.espressif/tools/openocd-esp32/v0.12.0-esp32-20250707
Installing esp-rom-elfs@20241011
Downloading https://github.com/espressif/esp-rom-elfs/releases/download/20241011/esp-rom-elfs-20241011.tar.gz
Destination: /home/johnpaul/.espressif/dist/esp-rom-elfs-20241011.tar.gz.tmp
Done
Extracting /home/johnpaul/.espressif/dist/esp-rom-elfs-20241011.tar.gz to /home/johnpaul/.espressif/tools/esp-rom-elfs/20241011
Installing Python environment and packages
Creating a new Python environment in /home/johnpaul/.espressif/python_env/idf5.5_py3.13_env
Downloading https://dl.espressif.com/dl/esp-idf/espidf.constraints.v5.5.txt
Destination: /home/johnpaul/.espressif/espidf.constraints.v5.5.txt.tmp
Done
Upgrading pip...
Requirement already satisfied: pip in /home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/lib/python3.13/site-packages (25.3)
Upgrading setuptools...
Collecting setuptools
  Using cached setuptools-80.9.0-py3-none-any.whl.metadata (6.6 kB)
Using cached setuptools-80.9.0-py3-none-any.whl (1.2 MB)
Installing collected packages: setuptools
Successfully installed setuptools-80.9.0
Installing Python packages
 Constraint file: /home/johnpaul/.espressif/espidf.constraints.v5.5.txt
 Requirement files:
  - /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt
Looking in indexes: https://pypi.org/simple, https://dl.espressif.com/pypi
Ignoring importlib_metadata: markers 'python_version < "3.8"' don't match your environment
Requirement already satisfied: setuptools in /home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/lib/python3.13/site-packages (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 7)) (80.9.0)
Collecting packaging (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 8))
  Downloading packaging-25.0-py3-none-any.whl.metadata (3.3 kB)
Collecting click (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 11))
  Downloading https://dl.espressif.com/pypi/click/click-8.1.8-py3-none-any.whl (98 kB)
Collecting pyserial (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 12))
  Downloading https://dl.espressif.com/pypi/pyserial/pyserial-3.5-py2.py3-none-any.whl (90 kB)
Collecting cryptography (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 13))
  Downloading cryptography-44.0.3-cp39-abi3-manylinux_2_34_x86_64.whl.metadata (5.7 kB)
Collecting pyparsing (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 14))
  Downloading pyparsing-3.2.5-py3-none-any.whl.metadata (5.0 kB)
Collecting pyelftools (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 15))
  Downloading https://dl.espressif.com/pypi/pyelftools/pyelftools-0.32-py3-none-any.whl (188 kB)
Collecting idf-component-manager~=2.2 (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading idf_component_manager-2.4.2-py3-none-any.whl.metadata (9.5 kB)
Collecting esp-coredump (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 17))
  Downloading esp_coredump-1.14.0-py3-none-any.whl.metadata (5.2 kB)
Collecting esptool (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading esptool-4.11.dev1.tar.gz (474 kB)
  Installing build dependencies ... done
  Getting requirements to build wheel ... done
  Preparing metadata (pyproject.toml) ... done
Collecting esp-idf-kconfig (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 19))
  Downloading https://dl.espressif.com/pypi/esp-idf-kconfig/esp_idf_kconfig-2.5.0-py3-none-any.whl (143 kB)
Collecting esp-idf-monitor (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 20))
  Downloading esp_idf_monitor-1.8.0-py3-none-any.whl.metadata (11 kB)
Collecting esp-idf-nvs-partition-gen (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 21))
  Downloading esp_idf_nvs_partition_gen-0.1.9-py3-none-any.whl.metadata (14 kB)
Collecting esp-idf-size (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 22))
  Downloading esp_idf_size-1.7.1-py3-none-any.whl.metadata (3.7 kB)
Collecting esp-idf-diag (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 23))
  Downloading esp_idf_diag-0.2.0-py3-none-any.whl.metadata (3.6 kB)
Collecting esp-idf-panic-decoder (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 24))
  Downloading esp_idf_panic_decoder-1.4.2-py3-none-any.whl.metadata (16 kB)
Collecting pyclang (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 25))
  Downloading pyclang-0.6.3-py3-none-any.whl.metadata (2.7 kB)
Collecting construct (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 26))
  Downloading https://dl.espressif.com/pypi/construct/construct-2.10.70-py3-none-any.whl (63 kB)
Collecting rich (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 27))
  Downloading https://dl.espressif.com/pypi/rich/rich-14.2.0-py3-none-any.whl (243 kB)
Collecting psutil (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 28))
  Downloading psutil-7.2.1-cp36-abi3-manylinux2010_x86_64.manylinux_2_12_x86_64.manylinux_2_28_x86_64.whl.metadata (22 kB)
Collecting tree_sitter (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 31))
  Downloading tree_sitter-0.25.2-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl.metadata (10.0 kB)
Collecting tree_sitter_c (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 32))
  Downloading tree_sitter_c-0.24.1-cp310-abi3-manylinux_2_5_x86_64.manylinux1_x86_64.manylinux_2_17_x86_64.manylinux2014_x86_64.whl.metadata (1.8 kB)
Collecting freertos_gdb (from -r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 35))
  Downloading https://dl.espressif.com/pypi/freertos-gdb/freertos_gdb-1.0.4-py3-none-any.whl (14 kB)
Collecting colorama (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/colorama/colorama-0.4.6-py2.py3-none-any.whl (25 kB)
Collecting ruamel.yaml (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading ruamel_yaml-0.18.17-py3-none-any.whl.metadata (27 kB)
Collecting requests (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Using cached requests-2.32.5-py3-none-any.whl.metadata (4.9 kB)
Collecting requests-file (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading requests_file-3.0.1-py2.py3-none-any.whl.metadata (1.7 kB)
Collecting requests-toolbelt (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/requests-toolbelt/requests_toolbelt-1.0.0-py2.py3-none-any.whl (54 kB)
Collecting tqdm (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/tqdm/tqdm-4.67.1-py3-none-any.whl (78 kB)
Collecting jsonref (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/jsonref/jsonref-1.1.0-py3-none-any.whl (9.4 kB)
Collecting pydantic!=2.12.0 (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading pydantic-2.12.5-py3-none-any.whl.metadata (90 kB)
Collecting pydantic-core (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading pydantic_core-2.41.5-cp313-cp313-manylinux_2_17_x86_64.manylinux2014_x86_64.whl.metadata (7.3 kB)
Collecting pydantic-settings (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading pydantic_settings-2.12.0-py3-none-any.whl.metadata (3.4 kB)
Collecting truststore (from idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading truststore-0.10.4-py3-none-any.whl.metadata (4.4 kB)
Collecting cffi>=1.12 (from cryptography->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 13))
  Downloading cffi-2.0.0-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.whl.metadata (2.6 kB)
Collecting pygdbmi>=0.9.0.2 (from esp-coredump->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 17))
  Downloading https://dl.espressif.com/pypi/pygdbmi/pygdbmi-0.11.0.0-py3-none-any.whl (21 kB)
Collecting bitstring!=4.2.0,>=3.1.6 (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading https://dl.espressif.com/pypi/bitstring/bitstring-4.3.1-py3-none-any.whl (71 kB)
Collecting ecdsa>=0.16.0 (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading https://dl.espressif.com/pypi/ecdsa/ecdsa-0.19.1-py2.py3-none-any.whl (150 kB)
Collecting reedsolo<1.8,>=1.5.3 (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading https://dl.espressif.com/pypi/reedsolo/reedsolo-1.7.0-py3-none-any.whl (32 kB)
Collecting PyYAML>=5.1 (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading pyyaml-6.0.3-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl.metadata (2.4 kB)
Collecting intelhex (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading https://dl.espressif.com/pypi/intelhex/intelhex-2.3.0-py2.py3-none-any.whl (50 kB)
Collecting argcomplete>=3 (from esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading argcomplete-3.6.3-py3-none-any.whl.metadata (16 kB)
Collecting markdown-it-py>=2.2.0 (from rich->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 27))
  Downloading https://dl.espressif.com/pypi/markdown-it-py/markdown_it_py-4.0.0-py3-none-any.whl (87 kB)
Collecting pygments<3.0.0,>=2.13.0 (from rich->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 27))
  Downloading https://dl.espressif.com/pypi/pygments/pygments-2.19.2-py3-none-any.whl (1.2 MB)
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 1.2/1.2 MB 628.3 kB/s  0:00:01
Collecting bitarray<4.0,>=3.0.0 (from bitstring!=4.2.0,>=3.1.6->esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading bitarray-3.8.0-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl.metadata (34 kB)
Collecting pycparser (from cffi>=1.12->cryptography->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 13))
  Downloading https://dl.espressif.com/pypi/pycparser/pycparser-2.23-py3-none-any.whl (118 kB)
Collecting six>=1.9.0 (from ecdsa>=0.16.0->esptool->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 18))
  Downloading https://dl.espressif.com/pypi/six/six-1.17.0-py2.py3-none-any.whl (11 kB)
Collecting mdurl~=0.1 (from markdown-it-py>=2.2.0->rich->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 27))
  Downloading https://dl.espressif.com/pypi/mdurl/mdurl-0.1.2-py3-none-any.whl (10.0 kB)
Collecting annotated-types>=0.6.0 (from pydantic!=2.12.0->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/annotated-types/annotated_types-0.7.0-py3-none-any.whl (13 kB)
Collecting typing-extensions>=4.14.1 (from pydantic!=2.12.0->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/typing-extensions/typing_extensions-4.15.0-py3-none-any.whl (44 kB)
Collecting typing-inspection>=0.4.2 (from pydantic!=2.12.0->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/typing-inspection/typing_inspection-0.4.2-py3-none-any.whl (14 kB)
Collecting python-dotenv>=0.21.0 (from pydantic-settings->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/python-dotenv/python_dotenv-1.2.1-py3-none-any.whl (21 kB)
Collecting charset_normalizer<4,>=2 (from requests->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Using cached charset_normalizer-3.4.4-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl.metadata (37 kB)
Collecting idna<4,>=2.5 (from requests->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/idna/idna-3.11-py3-none-any.whl (71 kB)
Collecting urllib3<3,>=1.21.1 (from requests->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading https://dl.espressif.com/pypi/urllib3/urllib3-1.26.20-py2.py3-none-any.whl (144 kB)
Collecting certifi>=2017.4.17 (from requests->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Using cached certifi-2025.11.12-py3-none-any.whl.metadata (2.5 kB)
Collecting ruamel.yaml.clib>=0.2.15 (from ruamel.yaml->idf-component-manager~=2.2->-r /home/johnpaul/esp/esp-idf/tools/requirements/requirements.core.txt (line 16))
  Downloading ruamel_yaml_clib-0.2.15-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl.metadata (3.5 kB)
Downloading idf_component_manager-2.4.2-py3-none-any.whl (172 kB)
Downloading packaging-25.0-py3-none-any.whl (66 kB)
Downloading cryptography-44.0.3-cp39-abi3-manylinux_2_34_x86_64.whl (4.2 MB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 4.2/4.2 MB 468.0 kB/s  0:00:08
Downloading pyparsing-3.2.5-py3-none-any.whl (113 kB)
Downloading esp_coredump-1.14.0-py3-none-any.whl (42 kB)
Downloading esp_idf_monitor-1.8.0-py3-none-any.whl (54 kB)
Downloading esp_idf_panic_decoder-1.4.2-py3-none-any.whl (21 kB)
Downloading esp_idf_nvs_partition_gen-0.1.9-py3-none-any.whl (22 kB)
Downloading esp_idf_size-1.7.1-py3-none-any.whl (82 kB)
Downloading esp_idf_diag-0.2.0-py3-none-any.whl (23 kB)
Downloading pyclang-0.6.3-py3-none-any.whl (12 kB)
Downloading psutil-7.2.1-cp36-abi3-manylinux2010_x86_64.manylinux_2_12_x86_64.manylinux_2_28_x86_64.whl (154 kB)
Downloading tree_sitter-0.25.2-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl (636 kB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 636.3/636.3 kB 390.1 kB/s  0:00:01
Downloading tree_sitter_c-0.24.1-cp310-abi3-manylinux_2_5_x86_64.manylinux1_x86_64.manylinux_2_17_x86_64.manylinux2014_x86_64.whl (98 kB)
Downloading argcomplete-3.6.3-py3-none-any.whl (43 kB)
Downloading bitarray-3.8.0-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl (340 kB)
Downloading cffi-2.0.0-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.whl (219 kB)
Downloading pydantic-2.12.5-py3-none-any.whl (463 kB)
Downloading pydantic_core-2.41.5-cp313-cp313-manylinux_2_17_x86_64.manylinux2014_x86_64.whl (2.1 MB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 2.1/2.1 MB 405.2 kB/s  0:00:05
Downloading pyyaml-6.0.3-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl (801 kB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 801.6/801.6 kB 398.6 kB/s  0:00:01
Downloading pydantic_settings-2.12.0-py3-none-any.whl (51 kB)
Using cached requests-2.32.5-py3-none-any.whl (64 kB)
Using cached charset_normalizer-3.4.4-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl (153 kB)
Using cached certifi-2025.11.12-py3-none-any.whl (159 kB)
Downloading requests_file-3.0.1-py2.py3-none-any.whl (4.5 kB)
Downloading ruamel_yaml-0.18.17-py3-none-any.whl (121 kB)
Downloading ruamel_yaml_clib-0.2.15-cp313-cp313-manylinux2014_x86_64.manylinux_2_17_x86_64.manylinux_2_28_x86_64.whl (782 kB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 782.1/782.1 kB 586.2 kB/s  0:00:01
Downloading truststore-0.10.4-py3-none-any.whl (18 kB)
Building wheels for collected packages: esptool
  Building wheel for esptool (pyproject.toml) ... done
  Created wheel for esptool: filename=esptool-4.11.dev1-py3-none-any.whl size=599165 sha256=da2724f53e47c7e97336d4b6dc0dafd6d93d6941efa3a52588500b8407e5d41b
  Stored in directory: /home/johnpaul/.cache/pip/wheels/89/91/44/a08346a3f586cf377fa5b228119abdd01099eef4a4bf30bc51
Successfully built esptool
Installing collected packages: reedsolo, pyserial, pygdbmi, pyelftools, intelhex, bitarray, urllib3, typing-extensions, truststore, tree_sitter_c, tree_sitter, tqdm, six, ruamel.yaml.clib, PyYAML, python-dotenv, pyparsing, pygments, pycparser, pyclang, psutil, packaging, mdurl, jsonref, idna, freertos_gdb, esp-idf-panic-decoder, esp-idf-kconfig, construct, colorama, click, charset_normalizer, certifi, bitstring, argcomplete, annotated-types, typing-inspection, ruamel.yaml, requests, pydantic-core, markdown-it-py, ecdsa, cffi, rich, requests-toolbelt, requests-file, pydantic, cryptography, pydantic-settings, esptool, esp-idf-size, esp-idf-nvs-partition-gen, esp-idf-diag, idf-component-manager, esp-coredump, esp-idf-monitor
Successfully installed PyYAML-6.0.3 annotated-types-0.7.0 argcomplete-3.6.3 bitarray-3.8.0 bitstring-4.3.1 certifi-2025.11.12 cffi-2.0.0 charset_normalizer-3.4.4 click-8.1.8 colorama-0.4.6 construct-2.10.70 cryptography-44.0.3 ecdsa-0.19.1 esp-coredump-1.14.0 esp-idf-diag-0.2.0 esp-idf-kconfig-2.5.0 esp-idf-monitor-1.8.0 esp-idf-nvs-partition-gen-0.1.9 esp-idf-panic-decoder-1.4.2 esp-idf-size-1.7.1 esptool-4.11.dev1 freertos_gdb-1.0.4 idf-component-manager-2.4.2 idna-3.11 intelhex-2.3.0 jsonref-1.1.0 markdown-it-py-4.0.0 mdurl-0.1.2 packaging-25.0 psutil-7.2.1 pyclang-0.6.3 pycparser-2.23 pydantic-2.12.5 pydantic-core-2.41.5 pydantic-settings-2.12.0 pyelftools-0.32 pygdbmi-0.11.0.0 pygments-2.19.2 pyparsing-3.2.5 pyserial-3.5 python-dotenv-1.2.1 reedsolo-1.7.0 requests-2.32.5 requests-file-3.0.1 requests-toolbelt-1.0.0 rich-14.2.0 ruamel.yaml-0.18.17 ruamel.yaml.clib-0.2.15 six-1.17.0 tqdm-4.67.1 tree_sitter-0.25.2 tree_sitter_c-0.24.1 truststore-0.10.4 typing-extensions-4.15.0 typing-inspection-0.4.2 urllib3-1.26.20
All done! You can now run:

  . /home/johnpaul/esp/esp-idf/export.fish
```

```shell
. $HOME/esp/esp-idf/export.fish
```
```
Checking "python3" ...
Python 3.13.11
python3 has been detected
Activating ESP-IDF 5.5
Setting IDF_PATH to '/home/johnpaul/esp/esp-idf'.
* Checking python version ... 3.13.11
* Checking python dependencies ... OK
* Deactivating the current ESP-IDF environment (if any) ... OK
* Establishing a new ESP-IDF environment ... OK
* Identifying shell ... fish
* Detecting outdated tools in system ... OK - no outdated tools found
* Shell completion ... OK

Done! You can now compile ESP-IDF projects.
Go to the project directory and run:

  idf.py build
```

```shell
cd ~/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch
idf.py set-target esp32c6
```
```
Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
Build directory '/home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build' not found. Nothing to clean.
Executing action: set-target
Set Target to: esp32c6, new sdkconfig will be created.
Running cmake in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "cmake -G Ninja -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python -DESP_PLATFORM=1 -DIDF_TARGET=esp32c6 -DCCACHE_ENABLE=0 /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch"...
-- Found Git: /usr/bin/git (found version "2.52.0")
-- Minimal build - OFF
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- The ASM compiler identification is GNU
-- Found assembler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-gcc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Building ESP-IDF components for target esp32c6
NOTICE: Updating lock file at /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/dependencies.lock
NOTICE: Processing 3 dependencies:
NOTICE: [1/3] espressif/esp-zboss-lib (1.6.4)
NOTICE: [2/3] espressif/esp-zigbee-lib (1.6.8)
NOTICE: [3/3] idf (5.5.2)
-- Project sdkconfig file /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/sdkconfig
Loading defaults file /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/sdkconfig.defaults...
-- Adding linker script /home/johnpaul/esp/esp-idf/components/riscv/ld/rom.api.ld
-- Found Python3: /home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python (found version "3.13.11") found components: Interpreter
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Performing Test C_COMPILER_SUPPORTS_WFORMAT_SIGNEDNESS
-- Performing Test C_COMPILER_SUPPORTS_WFORMAT_SIGNEDNESS - Success
-- USING O3
-- App "on_off_switch" version: 3d86dd0
-- Adding linker script /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/esp-idf/esp_system/ld/memory.ld
-- Adding linker script /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/esp-idf/esp_system/ld/sections.ld.in
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.api.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.rvfp.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.wdt.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.systimer.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.version.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.phy.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.coexist.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.net80211.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.pp.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.libc.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.libc-suboptimal_for_misaligned_mem.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.newlib.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.newlib-normal.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.heap.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/soc/esp32c6/ld/esp32c6.peripherals.ld
30aaf64524-3d86dd0-1.6.4-2025-12-31 07:41:38 UTC
-- Components: app_trace app_update bootloader bootloader_support bt cmock console cxx driver efuse esp-tls esp_adc esp_app_format esp_bootloader_format esp_coex esp_common esp_driver_ana_cmpr esp_driver_bitscrambler esp_driver_cam esp_driver_dac esp_driver_gpio esp_driver_gptimer esp_driver_i2c esp_driver_i2s esp_driver_isp esp_driver_jpeg esp_driver_ledc esp_driver_mcpwm esp_driver_parlio esp_driver_pcnt esp_driver_ppa esp_driver_rmt esp_driver_sdio esp_driver_sdm esp_driver_sdmmc esp_driver_sdspi esp_driver_spi esp_driver_touch_sens esp_driver_tsens esp_driver_twai esp_driver_uart esp_driver_usb_serial_jtag esp_eth esp_event esp_gdbstub esp_hid esp_http_client esp_http_server esp_https_ota esp_https_server esp_hw_support esp_lcd esp_local_ctrl esp_mm esp_netif esp_netif_stack esp_partition esp_phy esp_pm esp_psram esp_ringbuf esp_rom esp_security esp_system esp_tee esp_timer esp_vfs_console esp_wifi espcoredump espressif__esp-zboss-lib espressif__esp-zigbee-lib esptool_py fatfs freertos hal heap http_parser idf_test ieee802154 json log lwip main mbedtls mqtt newlib nvs_flash nvs_sec_provider openthread partition_table protobuf-c protocomm pthread riscv rt sdmmc soc spi_flash spiffs switch_driver tcp_transport ulp unity usb vfs wear_levelling wifi_provisioning wpa_supplicant
-- Component paths: /home/johnpaul/esp/esp-idf/components/app_trace /home/johnpaul/esp/esp-idf/components/app_update /home/johnpaul/esp/esp-idf/components/bootloader /home/johnpaul/esp/esp-idf/components/bootloader_support /home/johnpaul/esp/esp-idf/components/bt /home/johnpaul/esp/esp-idf/components/cmock /home/johnpaul/esp/esp-idf/components/console /home/johnpaul/esp/esp-idf/components/cxx /home/johnpaul/esp/esp-idf/components/driver /home/johnpaul/esp/esp-idf/components/efuse /home/johnpaul/esp/esp-idf/components/esp-tls /home/johnpaul/esp/esp-idf/components/esp_adc /home/johnpaul/esp/esp-idf/components/esp_app_format /home/johnpaul/esp/esp-idf/components/esp_bootloader_format /home/johnpaul/esp/esp-idf/components/esp_coex /home/johnpaul/esp/esp-idf/components/esp_common /home/johnpaul/esp/esp-idf/components/esp_driver_ana_cmpr /home/johnpaul/esp/esp-idf/components/esp_driver_bitscrambler /home/johnpaul/esp/esp-idf/components/esp_driver_cam /home/johnpaul/esp/esp-idf/components/esp_driver_dac /home/johnpaul/esp/esp-idf/components/esp_driver_gpio /home/johnpaul/esp/esp-idf/components/esp_driver_gptimer /home/johnpaul/esp/esp-idf/components/esp_driver_i2c /home/johnpaul/esp/esp-idf/components/esp_driver_i2s /home/johnpaul/esp/esp-idf/components/esp_driver_isp /home/johnpaul/esp/esp-idf/components/esp_driver_jpeg /home/johnpaul/esp/esp-idf/components/esp_driver_ledc /home/johnpaul/esp/esp-idf/components/esp_driver_mcpwm /home/johnpaul/esp/esp-idf/components/esp_driver_parlio /home/johnpaul/esp/esp-idf/components/esp_driver_pcnt /home/johnpaul/esp/esp-idf/components/esp_driver_ppa /home/johnpaul/esp/esp-idf/components/esp_driver_rmt /home/johnpaul/esp/esp-idf/components/esp_driver_sdio /home/johnpaul/esp/esp-idf/components/esp_driver_sdm /home/johnpaul/esp/esp-idf/components/esp_driver_sdmmc /home/johnpaul/esp/esp-idf/components/esp_driver_sdspi /home/johnpaul/esp/esp-idf/components/esp_driver_spi /home/johnpaul/esp/esp-idf/components/esp_driver_touch_sens /home/johnpaul/esp/esp-idf/components/esp_driver_tsens /home/johnpaul/esp/esp-idf/components/esp_driver_twai /home/johnpaul/esp/esp-idf/components/esp_driver_uart /home/johnpaul/esp/esp-idf/components/esp_driver_usb_serial_jtag /home/johnpaul/esp/esp-idf/components/esp_eth /home/johnpaul/esp/esp-idf/components/esp_event /home/johnpaul/esp/esp-idf/components/esp_gdbstub /home/johnpaul/esp/esp-idf/components/esp_hid /home/johnpaul/esp/esp-idf/components/esp_http_client /home/johnpaul/esp/esp-idf/components/esp_http_server /home/johnpaul/esp/esp-idf/components/esp_https_ota /home/johnpaul/esp/esp-idf/components/esp_https_server /home/johnpaul/esp/esp-idf/components/esp_hw_support /home/johnpaul/esp/esp-idf/components/esp_lcd /home/johnpaul/esp/esp-idf/components/esp_local_ctrl /home/johnpaul/esp/esp-idf/components/esp_mm /home/johnpaul/esp/esp-idf/components/esp_netif /home/johnpaul/esp/esp-idf/components/esp_netif_stack /home/johnpaul/esp/esp-idf/components/esp_partition /home/johnpaul/esp/esp-idf/components/esp_phy /home/johnpaul/esp/esp-idf/components/esp_pm /home/johnpaul/esp/esp-idf/components/esp_psram /home/johnpaul/esp/esp-idf/components/esp_ringbuf /home/johnpaul/esp/esp-idf/components/esp_rom /home/johnpaul/esp/esp-idf/components/esp_security /home/johnpaul/esp/esp-idf/components/esp_system /home/johnpaul/esp/esp-idf/components/esp_tee /home/johnpaul/esp/esp-idf/components/esp_timer /home/johnpaul/esp/esp-idf/components/esp_vfs_console /home/johnpaul/esp/esp-idf/components/esp_wifi /home/johnpaul/esp/esp-idf/components/espcoredump /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/managed_components/espressif__esp-zboss-lib /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/managed_components/espressif__esp-zigbee-lib /home/johnpaul/esp/esp-idf/components/esptool_py /home/johnpaul/esp/esp-idf/components/fatfs /home/johnpaul/esp/esp-idf/components/freertos /home/johnpaul/esp/esp-idf/components/hal /home/johnpaul/esp/esp-idf/components/heap /home/johnpaul/esp/esp-idf/components/http_parser /home/johnpaul/esp/esp-idf/components/idf_test /home/johnpaul/esp/esp-idf/components/ieee802154 /home/johnpaul/esp/esp-idf/components/json /home/johnpaul/esp/esp-idf/components/log /home/johnpaul/esp/esp-idf/components/lwip /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/main /home/johnpaul/esp/esp-idf/components/mbedtls /home/johnpaul/esp/esp-idf/components/mqtt /home/johnpaul/esp/esp-idf/components/newlib /home/johnpaul/esp/esp-idf/components/nvs_flash /home/johnpaul/esp/esp-idf/components/nvs_sec_provider /home/johnpaul/esp/esp-idf/components/openthread /home/johnpaul/esp/esp-idf/components/partition_table /home/johnpaul/esp/esp-idf/components/protobuf-c /home/johnpaul/esp/esp-idf/components/protocomm /home/johnpaul/esp/esp-idf/components/pthread /home/johnpaul/esp/esp-idf/components/riscv /home/johnpaul/esp/esp-idf/components/rt /home/johnpaul/esp/esp-idf/components/sdmmc /home/johnpaul/esp/esp-idf/components/soc /home/johnpaul/esp/esp-idf/components/spi_flash /home/johnpaul/esp/esp-idf/components/spiffs /home/johnpaul/esp/esp-zigbee-sdk/examples/common/switch_driver /home/johnpaul/esp/esp-idf/components/tcp_transport /home/johnpaul/esp/esp-idf/components/ulp /home/johnpaul/esp/esp-idf/components/unity /home/johnpaul/esp/esp-idf/components/usb /home/johnpaul/esp/esp-idf/components/vfs /home/johnpaul/esp/esp-idf/components/wear_levelling /home/johnpaul/esp/esp-idf/components/wifi_provisioning /home/johnpaul/esp/esp-idf/components/wpa_supplicant
-- Configuring done (94.0s)
-- Generating done (0.8s)
-- Build files have been written to: /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
```

```shell
idf.py erase-flash
```
```
Executing action: erase-flash
Serial port /dev/ttyACM0
Connecting...
Detecting chip type... ESP32-C6
Running esptool.py in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python /home/johnpaul/esp/esp-idf/components/esptool_py/esptool/esptool.py -p /dev/ttyACM0 -b 460800 --before default_reset --after hard_reset --chip esp32c6 erase_flash"...
esptool.py v4.11.dev1
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-C6FH4 (QFN32) (revision v0.2)
Features: Wi-Fi 6, BT 5 (LE), IEEE802.15.4, Single Core + LP Core, 160MHz, Embedded Flash 4MB
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: ff:ff:ff:ff:ff:ff:ff:ff
BASE MAC: ff:ff:ff:ff:ff:ff
MAC_EXT: ff:fe
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Erasing flash (this may take a while)...
Chip erase completed successfully in 2.0 seconds.
Hard resetting via RTS pin...
Done
```

```shell
idf.py -p /dev/ttyACM0 flash
```
```
Executing action: flash
Running ninja in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "ninja flash"...
[4/1062] Generating ../../partition_table/partition-table.bin
Partition table binary generated. Contents:
*******************************************************************************
# ESP-IDF Partition Table
# Name, Type, SubType, Offset, Size, Flags
nvs,data,nvs,0x9000,24K,
phy_init,data,phy,0xf000,4K,
factory,app,factory,0x10000,900K,
zb_storage,data,fat,0xf1000,16K,
zb_fct,data,fat,0xf5000,1K,
*******************************************************************************
[995/1062] Performing configure step for 'bootloader'
-- Found Git: /usr/bin/git (found version "2.52.0")
-- Minimal build - OFF
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- The ASM compiler identification is GNU
-- Found assembler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-gcc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /home/johnpaul/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20251107/riscv32-esp-elf/bin/riscv32-esp-elf-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Building ESP-IDF components for target esp32c6
-- Project sdkconfig file /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/sdkconfig
-- Adding linker script /home/johnpaul/esp/esp-idf/components/riscv/ld/rom.api.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/soc/esp32c6/ld/esp32c6.peripherals.ld
-- Bootloader project name: "bootloader" version: 1
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.api.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.rvfp.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.wdt.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.systimer.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.version.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.libc.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.libc-suboptimal_for_misaligned_mem.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/esp_rom/esp32c6/ld/esp32c6.rom.newlib.ld
-- Adding linker script /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main/ld/esp32c6/bootloader.rom.ld
-- Components: bootloader bootloader_support efuse esp_app_format esp_bootloader_format esp_common esp_hw_support esp_rom esp_security esp_system esp_tee esptool_py freertos hal log main micro-ecc newlib partition_table riscv soc spi_flash
-- Component paths: /home/johnpaul/esp/esp-idf/components/bootloader /home/johnpaul/esp/esp-idf/components/bootloader_support /home/johnpaul/esp/esp-idf/components/efuse /home/johnpaul/esp/esp-idf/components/esp_app_format /home/johnpaul/esp/esp-idf/components/esp_bootloader_format /home/johnpaul/esp/esp-idf/components/esp_common /home/johnpaul/esp/esp-idf/components/esp_hw_support /home/johnpaul/esp/esp-idf/components/esp_rom /home/johnpaul/esp/esp-idf/components/esp_security /home/johnpaul/esp/esp-idf/components/esp_system /home/johnpaul/esp/esp-idf/components/esp_tee /home/johnpaul/esp/esp-idf/components/esptool_py /home/johnpaul/esp/esp-idf/components/freertos /home/johnpaul/esp/esp-idf/components/hal /home/johnpaul/esp/esp-idf/components/log /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main /home/johnpaul/esp/esp-idf/components/bootloader/subproject/components/micro-ecc /home/johnpaul/esp/esp-idf/components/newlib /home/johnpaul/esp/esp-idf/components/partition_table /home/johnpaul/esp/esp-idf/components/riscv /home/johnpaul/esp/esp-idf/components/soc /home/johnpaul/esp/esp-idf/components/spi_flash
-- Adding linker script /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main/ld/esp32c6/bootloader.ld
-- Configuring done (5.1s)
-- Generating done (0.1s)
-- Build files have been written to: /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader
[123/124] Generating binary image from built executable
esptool.py v4.11.dev1
Creating esp32c6 image...
Merged 2 ELF sections
Successfully created esp32c6 image.
Generated /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bin
[124/124] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/.../esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bi
Bootloader binary size 0x5840 bytes. 0x27c0 bytes (31%) free.
[1060/1062] Generating binary image from built executable
esptool.py v4.11.dev1
Creating esp32c6 image...
Merged 1 ELF section
Successfully created esp32c6 image.
Generated /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bin
[1061/1062] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switc...paul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bi
on_off_switch.bin binary size 0x94200 bytes. Smallest app partition is 0xe1000 bytes. 0x4ce00 bytes (34%) free.
[1061/1062] cd /home/johnpaul/esp/esp-idf/components/esptool_py && /usr/bin/cmake -D IDF_PATH=..._on_off_switch/build -P /home/johnpaul/esp/esp-idf/components/esptool_py/run_serial_tool.cmak
esptool.py --chip esp32c6 -p /dev/ttyACM0 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 bootloader/bootloader.bin 0x10000 on_off_switch.bin 0x8000 partition_table/partition-table.bin
esptool.py v4.11.dev1
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-C6FH4 (QFN32) (revision v0.2)
Features: Wi-Fi 6, BT 5 (LE), IEEE802.15.4, Single Core + LP Core, 160MHz, Embedded Flash 4MB
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: ff:ff:ff:ff:ff:ff:ff:ff
BASE MAC: ff:ff:ff:ff:ff:ff
MAC_EXT: ff:fe
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00005fff...
Flash will be erased from 0x00010000 to 0x000a4fff...
Flash will be erased from 0x00008000 to 0x00008fff...
SHA digest in image updated
Compressed 22592 bytes to 13966...
Writing at 0x00000000... (100 %)
Wrote 22592 bytes (13966 compressed) at 0x00000000 in 0.1 seconds (effective 2307.4 kbit/s)...
Hash of data verified.
Compressed 606720 bytes to 363466...
Writing at 0x000a2405... (100 %)
Wrote 606720 bytes (363466 compressed) at 0x00010000 in 1.4 seconds (effective 3506.5 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 136...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (136 compressed) at 0x00008000 in 0.0 seconds (effective 2896.8 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Done
```

```shell
idf.py -p /dev/ttyACM0 monitor
```
```
Executing action: monitor
Running idf_monitor in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch
Executing "/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python /home/johnpaul/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyACM0 -b 115200 --toolchain-prefix riscv32-esp-elf- --target esp32c6 --revision 0 --decode-panic backtrace /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.elf /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.elf -m '/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python' '/home/johnpaul/esp/esp-idf/tools/idf.py' '-p' '/dev/ttyACM0'"...
--- esp-idf-monitor 1.8.0 on /dev/ttyACM0 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
I (106) esp_image: segment 1: paddr=0001efd4 vaddr=40800000 sizeESP-ROM:esp32c6-20220919
Build:Sep 19 2022
rst:0x15 (USB_UART_HPSYS),boot:0x1f (SPI_FAST_FLASH_BOOT)
Saved PC:0x4080515a
--- 0x4080515a: rv_utils_wait_for_intr at /home/johnpaul/esp/esp-idf/components/riscv/include/riscv/rv_utils.h:79
--- (inlined by) esp_cpu_wait_for_intr at /home/johnpaul/esp/esp-idf/components/esp_hw_support/cpu.c:62
SPIWP:0xee
mode:DIO, clock div:2
load:0x40875730,len:0x175c
load:0x4086b910,len:0xec8
load:0x4086e610,len:0x31c8
--- 0x4086b910: esp_bootloader_get_description at /home/johnpaul/esp/esp-idf/components/esp_bootloader_format/esp_bootloader_desc.c:39
--- 0x4086e610: esp_flash_encryption_enabled at /home/johnpaul/esp/esp-idf/components/bootloader_support/src/flash_encrypt.c:89
entry 0x4086b91a
--- 0x4086b91a: call_start_cpu0 at /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main/bootloader_start.c:25
I (23) boot: ESP-IDF v5.5.2 2nd stage bootloader
I (23) boot: compile time Dec 31 2025 07:44:04
I (24) boot: chip revision: v0.2
I (24) boot: efuse block revision: v0.3
I (26) boot.esp32c6: SPI Speed      : 80MHz
I (30) boot.esp32c6: SPI Mode       : DIO
I (34) boot.esp32c6: SPI Flash Size : 2MB
I (38) boot: Enabling RNG early entropy source...
I (42) boot: Partition Table:
I (45) boot: ## Label            Usage          Type ST Offset   Length
I (51) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (58) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (64) boot:  2 factory          factory app      00 00 00010000 000e1000
I (71) boot:  3 zb_storage       Unknown data     01 81 000f1000 00004000
I (77) boot:  4 zb_fct           Unknown data     01 81 000f5000 00000400
I (84) boot: End of partition table
I (87) esp_image: segment 0: paddr=00010020 vaddr=42078020 size=0efach ( 61356) map
I (106) esp_image: segment 1: paddr=0001efd4 vaddr=40800000 size=01044h (  4164) load
I (108) esp_image: segment 2: paddr=00020020 vaddr=42000020 size=72ea4h (470692) map
I (198) esp_image: segment 3: paddr=00092ecc vaddr=40801044 size=0edf4h ( 60916) load
I (212) esp_image: segment 4: paddr=000a1cc8 vaddr=4080fe40 size=02514h (  9492) load
I (219) boot: Loaded app from partition at offset 0x10000
I (219) boot: Disabling RNG early entropy source...
I (230) cpu_start: Unicore app
I (238) cpu_start: GPIO 17 and 16 are used as console UART I/O pins
I (239) cpu_start: Pro cpu start user code
I (239) cpu_start: cpu freq: 160000000 Hz
I (241) app_init: Application information:
I (244) app_init: Project name:     on_off_switch
I (249) app_init: App version:      3d86dd0
I (253) app_init: Compile time:     Dec 31 2025 07:43:50
I (258) app_init: ELF file SHA256:  f26aa5b76...
I (262) app_init: ESP-IDF:          v5.5.2
I (266) efuse_init: Min chip rev:     v0.0
I (270) efuse_init: Max chip rev:     v0.99
I (274) efuse_init: Chip rev:         v0.2
I (278) heap_init: Initializing. RAM available for dynamic allocation:
I (284) heap_init: At 40817220 len 000653F0 (404 KiB): RAM
I (289) heap_init: At 4087C610 len 00002F54 (11 KiB): RAM
I (294) heap_init: At 50000000 len 00003FE8 (15 KiB): RTCRAM
I (300) spi_flash: detected chip: generic
I (303) spi_flash: flash io: dio
W (306) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (319) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (325) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (340) main_task: Started on CPU0
I (340) main_task: Calling app_main()
I (350) phy_init: phy_version 343,b513b46,Nov 14 2025,16:34:12
I (430) phy: libbtbb version: c493933, Nov 14 2025, 16:34:25
I (440) main_task: Returned from app_main()
I (450) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Config Ready (0x17), status: ESP_FAIL
I (450) ESP_ZB_ON_OFF_SWITCH: Initialize Zigbee stack
W (450) ESP_ZB_ON_OFF_SWITCH: Network(0xf84a) closed, devices joining not allowed.
I (460) ESP_ZB_ON_OFF_SWITCH: Deferred driver initialization successful
I (460) ESP_ZB_ON_OFF_SWITCH: Device started up in non factory-reset mode
I (470) ESP_ZB_ON_OFF_SWITCH: Device rebooted
I (1070) ESP_ZB_ON_OFF_SWITCH: Network(0xf84a) is open for 180 seconds
W (181080) ESP_ZB_ON_OFF_SWITCH: Network(0xf84a) closed, devices joining not allowed.
```

```shell
idf.py -p /dev/ttyACM0 build flash monitor
```
```
Executing action: all (aliases: build)
Running ninja in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "ninja all"...
[1/4] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil...paul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bi
on_off_switch.bin binary size 0x94200 bytes. Smallest app partition is 0xe1000 bytes. 0x4ce00 bytes (34%) free.
[1/1] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil.../esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bi
Bootloader binary size 0x5840 bytes. 0x27c0 bytes (31%) free.
[4/4] Completed 'bootloader'Executing action: flash
Running ninja in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "ninja flash"...
[1/5] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil...paul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bi
on_off_switch.bin binary size 0x94200 bytes. Smallest app partition is 0xe1000 bytes. 0x4ce00 bytes (34%) free.
[1/1] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil.../esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bi
Bootloader binary size 0x5840 bytes. 0x27c0 bytes (31%) free.
[4/5] cd /home/johnpaul/esp/esp-idf/components/esptool_py && /usr/bin/cmake -D IDF_PATH=/home/..._on_off_switch/build -P /home/johnpaul/esp/esp-idf/components/esptool_py/run_serial_tool.cmak
esptool.py --chip esp32c6 -p /dev/ttyACM0 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 bootloader/bootloader.bin 0x10000 on_off_switch.bin 0x8000 partition_table/partition-table.bin
esptool.py v4.11.dev1
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-C6FH4 (QFN32) (revision v0.2)
Features: Wi-Fi 6, BT 5 (LE), IEEE802.15.4, Single Core + LP Core, 160MHz, Embedded Flash 4MB
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: ff:ff:ff:ff:ff:ff:ff:ff
BASE MAC: ff:ff:ff:ff:ff:ff
MAC_EXT: ff:fe
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00005fff...
Flash will be erased from 0x00010000 to 0x000a4fff...
Flash will be erased from 0x00008000 to 0x00008fff...
SHA digest in image updated
Compressed 22592 bytes to 13966...
Writing at 0x00000000... (100 %)
Wrote 22592 bytes (13966 compressed) at 0x00000000 in 0.1 seconds (effective 2304.2 kbit/s)...
Hash of data verified.
Compressed 606720 bytes to 363466...
Writing at 0x000a2405... (100 %)
Wrote 606720 bytes (363466 compressed) at 0x00010000 in 1.4 seconds (effective 3462.1 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 136...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (136 compressed) at 0x00008000 in 0.0 seconds (effective 2900.2 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Executing action: monitor
Running idf_monitor in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch
Executing "/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python /home/johnpaul/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyACM0 -b 115200 --toolchain-prefix riscv32-esp-elf- --target esp32c6 --revision 0 --decode-panic backtrace /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.elf /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.elf -m '/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python' '/home/johnpaul/esp/esp-idf/tools/idf.py' '-p' '/dev/ttyACM0'"...
--- esp-idf-monitor 1.8.0 on /dev/ttyACM0 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
I (106) esp_image: segment 1: paddr=0001efd4 vaddr=40800000 sizeESP-ROM:esp32c6-20220919
Build:Sep 19 2022
rst:0x15 (USB_UART_HPSYS),boot:0x1f (SPI_FAST_FLASH_BOOT)
Saved PC:0x40017604
--- 0x40017604: ets_delay_us in ROM
SPIWP:0xee
mode:DIO, clock div:2
load:0x40875730,len:0x175c
load:0x4086b910,len:0xec8
--- 0x4086b910: esp_bootloader_get_description at /home/johnpaul/esp/esp-idf/components/esp_bootloader_format/esp_bootloader_desc.c:39
load:0x4086e610,len:0x31c8
--- 0x4086e610: esp_flash_encryption_enabled at /home/johnpaul/esp/esp-idf/components/bootloader_support/src/flash_encrypt.c:89
entry 0x4086b91a
--- 0x4086b91a: call_start_cpu0 at /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main/bootloader_start.c:25
I (23) boot: ESP-IDF v5.5.2 2nd stage bootloader
I (23) boot: compile time Dec 31 2025 07:44:04
I (24) boot: chip revision: v0.2
I (24) boot: efuse block revision: v0.3
I (26) boot.esp32c6: SPI Speed      : 80MHz
I (30) boot.esp32c6: SPI Mode       : DIO
I (34) boot.esp32c6: SPI Flash Size : 2MB
I (38) boot: Enabling RNG early entropy source...
I (42) boot: Partition Table:
I (45) boot: ## Label            Usage          Type ST Offset   Length
I (51) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (58) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (64) boot:  2 factory          factory app      00 00 00010000 000e1000
I (71) boot:  3 zb_storage       Unknown data     01 81 000f1000 00004000
I (77) boot:  4 zb_fct           Unknown data     01 81 000f5000 00000400
I (84) boot: End of partition table
I (87) esp_image: segment 0: paddr=00010020 vaddr=42078020 size=0efach ( 61356) map
I (106) esp_image: segment 1: paddr=0001efd4 vaddr=40800000 size=01044h (  4164) load
I (108) esp_image: segment 2: paddr=00020020 vaddr=42000020 size=72ea4h (470692) map
I (198) esp_image: segment 3: paddr=00092ecc vaddr=40801044 size=0edf4h ( 60916) load
I (212) esp_image: segment 4: paddr=000a1cc8 vaddr=4080fe40 size=02514h (  9492) load
I (219) boot: Loaded app from partition at offset 0x10000
I (219) boot: Disabling RNG early entropy source...
I (230) cpu_start: Unicore app
I (238) cpu_start: GPIO 17 and 16 are used as console UART I/O pins
I (239) cpu_start: Pro cpu start user code
I (239) cpu_start: cpu freq: 160000000 Hz
I (241) app_init: Application information:
I (244) app_init: Project name:     on_off_switch
I (249) app_init: App version:      3d86dd0
I (253) app_init: Compile time:     Dec 31 2025 07:43:50
I (258) app_init: ELF file SHA256:  f26aa5b76...
I (262) app_init: ESP-IDF:          v5.5.2
I (266) efuse_init: Min chip rev:     v0.0
I (270) efuse_init: Max chip rev:     v0.99
I (274) efuse_init: Chip rev:         v0.2
I (278) heap_init: Initializing. RAM available for dynamic allocation:
I (284) heap_init: At 40817220 len 000653F0 (404 KiB): RAM
I (289) heap_init: At 4087C610 len 00002F54 (11 KiB): RAM
I (294) heap_init: At 50000000 len 00003FE8 (15 KiB): RTCRAM
I (300) spi_flash: detected chip: generic
I (303) spi_flash: flash io: dio
W (306) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (319) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (325) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (340) main_task: Started on CPU0
I (340) main_task: Calling app_main()
I (350) phy_init: phy_version 343,b513b46,Nov 14 2025,16:34:12
W (350) phy_init: failed to load RF calibration data (0x1102), falling back to full calibration
I (440) phy_init: Saving new calibration data due to checksum failure or outdated calibration data, mode(2)
I (440) phy: libbtbb version: c493933, Nov 14 2025, 16:34:25
I (460) main_task: Returned from app_main()
I (470) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Config Ready (0x17), status: ESP_FAIL
I (470) ESP_ZB_ON_OFF_SWITCH: Initialize Zigbee stack
I (470) ESP_ZB_ON_OFF_SWITCH: Deferred driver initialization successful
I (470) ESP_ZB_ON_OFF_SWITCH: Device started up in factory-reset mode
I (480) ESP_ZB_ON_OFF_SWITCH: Start network formation
W (630) ESP_ZB_ON_OFF_SWITCH: Network(0x09bc) closed, devices joining not allowed.
I (630) ESP_ZB_ON_OFF_SWITCH: Formed network successfully (Extended PAN ID: ff:ff:ff:ff:ff:ff:ff:ff, PAN ID: 0x09bc, Channel:13, Short Address: 0x0000)
I (1240) ESP_ZB_ON_OFF_SWITCH: Network(0x09bc) is open for 180 seconds
I (1240) ESP_ZB_ON_OFF_SWITCH: Network steering started
I (9790) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NWK Device Associated (0x12), status: ESP_OK
I (9800) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Device Update (0x30), status: ESP_OK
I (9930) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x3ce4)
I (9960) ESP_ZB_ON_OFF_SWITCH: Found light
I (9960) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (9960) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (9960) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x3ce4) on endpoint(1)
I (10530) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Device Authorized (0x2f), status: ESP_OK
I (13030) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x3ce4)
I (13060) ESP_ZB_ON_OFF_SWITCH: Found light
I (13060) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (13060) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (13060) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x3ce4) on endpoint(1)
I (16030) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x3ce4)
I (16060) ESP_ZB_ON_OFF_SWITCH: Found light
I (16060) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (16070) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (16070) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x3ce4) on endpoint(1)
I (18880) ESP_ZB_ON_OFF_SWITCH: Network(0x09bc) is open for 180 seconds
I (19390) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x3ce4)
I (19410) ESP_ZB_ON_OFF_SWITCH: Found light
I (19410) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (19420) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (19420) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x3ce4) on endpoint(1)
```

```shell
idf.py -p /dev/ttyACM0 build flash monitor
```
```
Executing action: all (aliases: build)
Running ninja in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "ninja all"...
[1/4] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil...paul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bi
on_off_switch.bin binary size 0x94b90 bytes. Smallest app partition is 0xe1000 bytes. 0x4c470 bytes (34%) free.
[1/1] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil.../esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bi
Bootloader binary size 0x5840 bytes. 0x27c0 bytes (31%) free.
[4/4] Completed 'bootloader'Executing action: flash
Running ninja in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build
Executing "ninja flash"...
[1/5] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil...paul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.bi
on_off_switch.bin binary size 0x94b90 bytes. Smallest app partition is 0xe1000 bytes. 0x4c470 bytes (34%) free.
[1/1] cd /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/buil.../esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.bi
Bootloader binary size 0x5840 bytes. 0x27c0 bytes (31%) free.
[4/5] cd /home/johnpaul/esp/esp-idf/components/esptool_py && /usr/bin/cmake -D IDF_PATH=/home/..._on_off_switch/build -P /home/johnpaul/esp/esp-idf/components/esptool_py/run_serial_tool.cmak
esptool.py --chip esp32c6 -p /dev/ttyACM0 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 bootloader/bootloader.bin 0x10000 on_off_switch.bin 0x8000 partition_table/partition-table.bin
esptool.py v4.11.dev1
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-C6FH4 (QFN32) (revision v0.2)
Features: Wi-Fi 6, BT 5 (LE), IEEE802.15.4, Single Core + LP Core, 160MHz, Embedded Flash 4MB
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: ff:ff:ff:ff:ff:ff:ff:ff
BASE MAC: ff:ff:ff:ff:ff:ff
MAC_EXT: ff:fe
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00005fff...
Flash will be erased from 0x00010000 to 0x000a4fff...
Flash will be erased from 0x00008000 to 0x00008fff...
SHA digest in image updated
Compressed 22592 bytes to 13966...
Writing at 0x00000000... (100 %)
Wrote 22592 bytes (13966 compressed) at 0x00000000 in 0.1 seconds (effective 2290.0 kbit/s)...
Hash of data verified.
Compressed 609168 bytes to 364928...
Writing at 0x000a1e9c... (100 %)
Wrote 609168 bytes (364928 compressed) at 0x00010000 in 1.4 seconds (effective 3563.6 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 136...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (136 compressed) at 0x00008000 in 0.0 seconds (effective 2875.6 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Executing action: monitor
Running idf_monitor in directory /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch
Executing "/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python /home/johnpaul/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyACM0 -b 115200 --toolchain-prefix riscv32-esp-elf- --target esp32c6 --revision 0 --decode-panic backtrace /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/on_off_switch.elf /home/johnpaul/esp/esp-zigbee-sdk/examples/esp_zigbee_HA_sample/HA_on_off_switch/build/bootloader/bootloader.elf -m '/home/johnpaul/.espressif/python_env/idf5.5_py3.13_env/bin/python' '/home/johnpaul/esp/esp-idf/tools/idf.py' '-p' '/dev/ttyACM0'"...
--- esp-idf-monitor 1.8.0 on /dev/ttyACM0 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
I (106) esp_image: segment 1: paddr=0001f180 vaddr=40800000 sizeI (344) main_task: Started on CPU0
ESP-ROM:esp32c6-20220919
Build:Sep 19 2022
rst:0x15 (USB_UART_HPSYS),boot:0x1f (SPI_FAST_FLASH_BOOT)
Saved PC:0x420077aa
--- 0x420077aa: uart_ll_get_txfifo_len at /home/johnpaul/esp/esp-idf/components/hal/esp32c6/include/hal/uart_ll.h:622
--- (inlined by) uart_tx_char at /home/johnpaul/esp/esp-idf/components/esp_driver_uart/src/uart_vfs.c:190
SPIWP:0xee
mode:DIO, clock div:2
load:0x40875730,len:0x175c
load:0x4086b910,len:0xec8
load:0x4086e610,len:0x31c8
--- 0x4086b910: esp_bootloader_get_description at /home/johnpaul/esp/esp-idf/components/esp_bootloader_format/esp_bootloader_desc.c:39
--- 0x4086e610: esp_flash_encryption_enabled at /home/johnpaul/esp/esp-idf/components/bootloader_support/src/flash_encrypt.c:89
entry 0x4086b91a
--- 0x4086b91a: call_start_cpu0 at /home/johnpaul/esp/esp-idf/components/bootloader/subproject/main/bootloader_start.c:25
I (23) boot: ESP-IDF v5.5.2 2nd stage bootloader
I (23) boot: compile time Dec 31 2025 07:44:04
I (24) boot: chip revision: v0.2
I (24) boot: efuse block revision: v0.3
I (26) boot.esp32c6: SPI Speed      : 80MHz
I (30) boot.esp32c6: SPI Mode       : DIO
I (34) boot.esp32c6: SPI Flash Size : 2MB
I (38) boot: Enabling RNG early entropy source...
I (42) boot: Partition Table:
I (45) boot: ## Label            Usage          Type ST Offset   Length
I (51) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (58) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (64) boot:  2 factory          factory app      00 00 00010000 000e1000
I (71) boot:  3 zb_storage       Unknown data     01 81 000f1000 00004000
I (77) boot:  4 zb_fct           Unknown data     01 81 000f5000 00000400
I (84) boot: End of partition table
I (87) esp_image: segment 0: paddr=00010020 vaddr=42078020 size=0f158h ( 61784) map
I (106) esp_image: segment 1: paddr=0001f180 vaddr=40800000 size=00e98h (  3736) load
I (108) esp_image: segment 2: paddr=00020020 vaddr=42000020 size=72fb4h (470964) map
I (198) esp_image: segment 3: paddr=00092fdc vaddr=40800e98 size=0f670h ( 63088) load
I (213) esp_image: segment 4: paddr=000a2654 vaddr=40810510 size=02514h (  9492) load
I (219) boot: Loaded app from partition at offset 0x10000
I (220) boot: Disabling RNG early entropy source...
I (230) cpu_start: Unicore app
I (239) cpu_start: GPIO 17 and 16 are used as console UART I/O pins
I (239) cpu_start: Pro cpu start user code
I (239) cpu_start: cpu freq: 160000000 Hz
I (241) app_init: Application information:
I (245) app_init: Project name:     on_off_switch
I (249) app_init: App version:      3d86dd0
I (253) app_init: Compile time:     Dec 31 2025 07:43:50
I (258) app_init: ELF file SHA256:  50dbed24d...
I (263) app_init: ESP-IDF:          v5.5.2
I (267) efuse_init: Min chip rev:     v0.0
I (270) efuse_init: Max chip rev:     v0.99
I (274) efuse_init: Chip rev:         v0.2
I (278) heap_init: Initializing. RAM available for dynamic allocation:
I (284) heap_init: At 40817A30 len 00064BE0 (402 KiB): RAM
I (290) heap_init: At 4087C610 len 00002F54 (11 KiB): RAM
I (295) heap_init: At 50000000 len 00003FE8 (15 KiB): RTCRAM
I (301) spi_flash: detected chip: generic
I (304) spi_flash: flash io: dio
W (307) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (319) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (325) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (340) main_task: Started on CPU0
I (340) main_task: Calling app_main()
I (350) phy_init: phy_version 343,b513b46,Nov 14 2025,16:34:12
W (350) phy_init: failed to load RF calibration data (0x1102), falling back to full calibration
I (440) phy_init: Saving new calibration data due to checksum failure or outdated calibration data, mode(2)
I (440) phy: libbtbb version: c493933, Nov 14 2025, 16:34:25
I (460) main_task: Returned from app_main()
I (470) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Config Ready (0x17), status: ESP_FAIL
I (470) ESP_ZB_ON_OFF_SWITCH: Initialize Zigbee stack
I (470) ESP_ZB_ON_OFF_SWITCH: Deferred driver initialization successful
I (480) ESP_ZB_ON_OFF_SWITCH: Device started up in factory-reset mode
I (480) ESP_ZB_ON_OFF_SWITCH: Start network formation
W (640) ESP_ZB_ON_OFF_SWITCH: Network(0xf939) closed, devices joining not allowed.
I (640) ESP_ZB_ON_OFF_SWITCH: Formed network successfully (Extended PAN ID: ff:ff:ff:ff:ff:ff:ff:ff, PAN ID: 0xf939, Channel:13, Short Address: 0x0000)
I (1250) ESP_ZB_ON_OFF_SWITCH: Network(0xf939) is open for 180 seconds
I (1250) ESP_ZB_ON_OFF_SWITCH: Network steering started
I (1250) ESP_ZB_ON_OFF_SWITCH: Started automatic light cycling (15s ON / 15s OFF)
I (1250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (16250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (19880) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NWK Device Associated (0x12), status: ESP_OK
I (19890) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Device Update (0x30), status: ESP_OK
I (20020) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x4dbf)
I (20050) ESP_ZB_ON_OFF_SWITCH: Found light
I (20050) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (20050) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (20060) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x4dbf) on endpoint(1)
I (20620) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Device Authorized (0x2f), status: ESP_OK
I (23120) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x4dbf)
I (23150) ESP_ZB_ON_OFF_SWITCH: Found light
I (23150) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (23150) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (23150) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x4dbf) on endpoint(1)
I (26120) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x4dbf)
I (26150) ESP_ZB_ON_OFF_SWITCH: Found light
I (26150) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (26150) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (26160) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x4dbf) on endpoint(1)
I (28970) ESP_ZB_ON_OFF_SWITCH: Network(0xf939) is open for 180 seconds
I (29480) ESP_ZB_ON_OFF_SWITCH: New device commissioned or rejoined (short: 0x4dbf)
I (29500) ESP_ZB_ON_OFF_SWITCH: Found light
I (29500) ESP_ZB_ON_OFF_SWITCH: Try to bind On/Off
I (29510) ESP_ZB_ON_OFF_SWITCH: Bound successfully!
I (29510) ESP_ZB_ON_OFF_SWITCH: The light originating from address(0x4dbf) on endpoint(1)
I (31250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (46250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (61250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (76250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (87280) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NLME Status Indication (0x32), status: ESP_OK
I (91250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (106250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (121250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (136250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (151250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (166250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (181250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (196250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
W (208980) ESP_ZB_ON_OFF_SWITCH: Network(0xf939) closed, devices joining not allowed.
I (211250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (226250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (241250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (256250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (271250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (286250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (301250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (316250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (301250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (316250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (331250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off off' command
I (331270) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NLME Status Indication (0x32), status: ESP_OK
I (334270) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NLME Status Indication (0x32), status: ESP_OK
I (337280) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NLME Status Indication (0x32), status: ESP_OK
I (346250) ESP_ZB_ON_OFF_SWITCH: Send 'on_off on' command
I (350830) ESP_ZB_ON_OFF_SWITCH: ZDO signal: NLME Status Indication (0x32), status: ESP_OK
I (352340) ESP_ZB_ON_OFF_SWITCH: ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
```

