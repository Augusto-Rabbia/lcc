        .section .rodata
str:    .string "0x%X\n"
num:    .word 0x11223344

        .text
        .arm
        .global main
main:
        push {lr}

        @ Opción 0
        mov  r1, #0x00000044
        add  r1, r1, #0x00003300
        add  r1, r1, #0x00220000
        add  r1, r1, #0x11000000
        ldr  r0, =str
        bl   printf

        @ Opción 1
        movw r1, #0x3344  @ Si es etiqueta: #:lower16:etiqueta
        movt r1, #0x1122  @ Si es etiqueta: #:upper16:etiqueta
        ldr  r0, =str
        bl   printf

        @ Opción 2
        ldr  r1, =0x11223344
        ldr  r0, =str
        bl   printf

        @ Opción 3
        ldr  r2, =num
        ldr  r1, [r2]
        ldr  r0, =str
        bl   printf

        pop  {lr}
        bx   lr
