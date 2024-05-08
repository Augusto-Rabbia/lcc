.global main
main:
    movb $100, %al # N1 = 100
    movb $120, %ah # N2 = 120,   eflags = [ PF ZF IF ]
    addb %ah, %al # %al = %al + %ah = 100 + 120 = 220,    eflags = [ SF IF OF]
    ret
