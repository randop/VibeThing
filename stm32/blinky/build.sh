#!/bin/sh

rm -f blinky.bin &&
  rm -f stm32*.elf &&
  make &&
  arm-none-eabi-objcopy -O binary stm32-blinky-f1.elf blinky.bin &&
  st-flash --reset erase &&
  st-flash --reset write blinky.bin 0x08000000
