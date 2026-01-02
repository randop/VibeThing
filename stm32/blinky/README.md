# Blinky

Demo of STM32 project for blinking the built-in LED

### Install toolchain
```shell
sudo pacman -Syy --needed base-devel
sudo pacman -S meson \
  ninja \
  cmake \
  make \
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
  -DBLINKY_F1_EXAMPLE=ON \
  -DSTM32_CUBE_F1_PATH=~/stm32/STM32CubeF1 \
  -DSTM32_CHIP=STM32F103x6
```

### Building
```shell
cd build
make -j$(nproc)
arm-none-eabi-objcopy -O binary stm32-blinky-f1.elf blinky.bin
```

### Flashing
Using [stlink](https://github.com/stlink-org/stlink) utility:
```shell
st-flash --reset write blinky.bin 0x08000000
```
or using [OpenOCD](https://openocd.org/)
```shell
openocd -f interface/stlink.cfg \
        -f target/stm32f1x.cfg \
        -c "program blinky.bin verify reset exit 0x08000000"
```

---

#### Linux dmesg logs
```
[11606.993043] usb 5-1: new full-speed USB device number 2 using xhci_hcd
[11607.145833] usb 5-1: New USB device found, idVendor=0483, idProduct=3748, bcdDevice= 1.00
[11607.145844] usb 5-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[11607.145849] usb 5-1: Product: STM32 STLink
[11607.145853] usb 5-1: Manufacturer: STMicroelectronics
[11607.145857] usb 5-1: SerialNumber: 000000000001
```

> This is a USB Memory Device demonstration for
>
> the Keil MCBSTM32 Board with ST STM32F103RBT6.

```
[13485.062044] usb 3-4: new full-speed USB device number 4 using xhci_hcd
[13485.215786] usb 3-4: New USB device found, idVendor=c251, idProduct=1c03, bcdDevice= 1.00
[13485.215796] usb 3-4: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[13485.215800] usb 3-4: Product: Keil MCBSTM32 Memory
[13485.215804] usb 3-4: Manufacturer: Keil Software
[13485.215808] usb 3-4: SerialNumber: 0001A0000000
[13485.245316] usb-storage 3-4:1.0: USB Mass Storage device detected
[13485.245524] scsi host9: usb-storage 3-4:1.0
[13485.245628] usbcore: registered new interface driver usb-storage
[13485.248211] usbcore: registered new interface driver uas
[13486.297123] scsi 9:0:0:0: Direct-Access     Keil     MCBSTM32 Disk    1.0  PQ: 0 ANSI: 0 CCS
[13486.297429] sd 9:0:0:0: Attached scsi generic sg2 type 0
[13486.298539] sd 9:0:0:0: [sdc] 16 512-byte logical blocks: (8.19 kB/8.00 KiB)
[13486.299867] sd 9:0:0:0: [sdc] Write Protect is off
[13486.299873] sd 9:0:0:0: [sdc] Mode Sense: 03 00 00 00
[13486.301853] sd 9:0:0:0: [sdc] No Caching mode page found
[13486.301860] sd 9:0:0:0: [sdc] Assuming drive cache: write through
[13486.311608]  sdc:
[13486.312005] sd 9:0:0:0: [sdc] Attached SCSI removable disk
```
