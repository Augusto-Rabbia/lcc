.data
str: .asciz "Hola mundo\n"

.text
.global main
main:
	# write(1, message, 12)
        mov     $1, %rax                # system call 1 is write
        mov     $1, %rdi                # file handle 1 is stdout
        lea     str, %rsi               # address of string to output
        mov     $12, %rdx               # number of bytes
        syscall

    # exit(0)
        mov     $60, %rax               # system call 60 is exit
        xor     %rdi, %rdi              # return code 0
        syscall
