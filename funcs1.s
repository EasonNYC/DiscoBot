  .section .text
  .syntax unified
  .global func1, func2

func1:
  PUSH {LR}
  BL func2
  POP {LR}
  BX LR         /* Return from the function */

func2:
  ADD R0,R0,#1
  BX LR



