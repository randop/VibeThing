.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.global  g_pfnVectors
.global  Reset_Handler

.section .isr_vector,"a",%progbits
.type  g_pfnVectors, %object
.size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  /* Rest of vector table can be empty for this minimal example */
  .word  0
  .word  0
  .word  0
  /* ... you can add more if needed later */

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* Set stack pointer */

  /* Zero .bss */
  ldr r0, =_sbss
  ldr r1, =_ebss
  mov r2, #0
  b LoopFillZerobss
FillZerobss:
  str r2, [r0], #4
LoopFillZerobss:
  cmp r0, r1
  bcc FillZerobss

  bl main
  bx lr
.size Reset_Handler, .-Reset_Handler
