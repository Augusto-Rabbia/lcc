.global main
main:
            MOV   r1, #0      @ inicializar r1
    
            B     loopTest    

loopStart:
   			ADD   r1, r1, #1  @ r1++
loopTest:
   			CMP   r1, #5      @ compara r1 con 5
   			BLT   loopStart   @ si r1 es menor que 5, vuelve a loopStart

   			bx lr
