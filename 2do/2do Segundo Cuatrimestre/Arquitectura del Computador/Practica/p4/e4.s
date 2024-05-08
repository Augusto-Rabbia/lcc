.global main
main:
    MOV r7, #0x8C, 4
    MOV r7, #0x42, 30
    MVN r7, #2
    MVN r7, #0x8C, 4
    bx lr
