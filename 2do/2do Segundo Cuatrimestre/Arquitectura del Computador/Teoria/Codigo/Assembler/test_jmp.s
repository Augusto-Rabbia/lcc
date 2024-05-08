        .text
        .arm
        .global main
main:
        
B forward
ADD r1, r2, #4
ADD r0, r6, #2
ADD r3, r7, #4
forward:
SUB r1, r2, #4
mov r0, #0
bx  lr
