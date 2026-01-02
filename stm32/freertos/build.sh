#!/bin/sh

rm -f *.bin &&
  rm -f stm32*.elf &&
  make &&
  arm-none-eabi-objcopy -O binary stm32-freertos-f100rb.elf freertos.bin &&
  st-flash --reset erase &&
  st-flash --reset write freertos.bin 0x08000000
