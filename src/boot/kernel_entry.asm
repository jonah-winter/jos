bits 32

section .text._start
global _start
extern k_main

_start:
  call k_main

.hang:
  cli
  hlt
  jmp .hang
