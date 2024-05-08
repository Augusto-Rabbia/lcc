.global f
f:
    mov r1, #1
    mov r0, r1, LSL r0
    bx lr

.global main
main:
    push {lr}

    mov r0, #0
    bl f

    mov r0, #5
    bl f

    mov r0, #6
    bl f

    mov r0, #31
    bl f

    pop {lr}
    bx lr
