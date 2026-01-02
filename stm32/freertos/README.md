# FreeRTOS

Demo of STM32 project for blinking the built-in LED using FreeRTOS

### Install toolchain
```shell
sudo pacman -Syy --needed base-devel
sudo pacman -S meson \
  ninja \
  cmake \
  make \
  clang \
  git \
  python \
  arm-none-eabi-gcc \
  arm-none-eabi-newlib \
  arm-none-eabi-gdb \
  arm-none-eabi-binutils \
  openocd \
  stlink
```

### Setup
```shell
mkdir -pv ~/stm32/STM32CubeF1
git clone --recursive https://github.com/STMicroelectronics/STM32CubeF1.git ~/stm32/STM32CubeF1
git clone https://github.com/ObKo/stm32-cmake ~/stm32/stm32-cmake

rm -rf build && mkdir build && cd build
cmake .. \
  -DFREERTOS_F100RB_EXAMPLE=ON \
  -DSTM32_CUBE_F1_PATH=~/stm32/STM32CubeF1 \
  -DSTM32_CHIP=STM32F103x6
```

### Building
```shell
cd build
make -j$(nproc)
arm-none-eabi-objcopy -O binary stm32-freertos-f100rb.elf freertos.bin
```

### Flashing
Using [stlink](https://github.com/stlink-org/stlink) utility:
```shell
st-flash --reset write freertos.bin 0x08000000
```
or using [OpenOCD](https://openocd.org/)
```shell
openocd -f interface/stlink.cfg \
        -f target/stm32f1x.cfg \
        -c "program freertos.bin verify reset exit 0x08000000"
```
