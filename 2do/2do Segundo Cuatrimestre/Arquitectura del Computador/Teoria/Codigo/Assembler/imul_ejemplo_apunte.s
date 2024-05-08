.text
.global main
    main:
    movq $0xffffffffffffffff, %rax
    movq $4, %rbx
    mulq %rbx  # Multiplica rbx*rax y el resultado queda en rdx:rax.
    # Entonces: rax=0xfffffffffffffffc y rdx=3
    movq $9, %rax
    movq $3, %rbx
    imulq %rbx, %rax                # rax=27
    imulq $4, %rax                   # rax=108
    imulq $2, %rax, %rbx            # rbx=216
    movq $5, %rax
    imulq $3, %rax, %rbx               # rbx=15
    ret
