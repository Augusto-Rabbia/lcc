/*
El programa está usando los registros 1, 4 y 5. Por lo tanto, es responsable de
 guardar/restaurar r1
*/

.text
.global main
main:
    MOV   r1, #0xAA
    MOV   r4, #30
    MOV   r5, #-12
    
    PUSH  {r1}          @ Se guarda r1
    MOV   r0, r4      	 @ Configurar parámetro para llamada a función
    BL    abs           @ Se llama a la función 	   	
    MOV   r4, r0      	 @ Salvar resultado a r4
    POP   {r1}        	 @ Restaurar r1
    
    PUSH  {r1}        	@ Se guarda r1
    MOV   r0, r5      	@ Configurar parámetro para llamada a función
    BL    abs          @ Se llama a la función
    MOV   r5, r0      	@ Salvar resultado 
    POP   {r1}        	@ Restaurar r1
  
    BX     lr 

abs:
    PUSH  {r4, r5}      @ Guarda registros calle saved 
    MOV   r1, #0
    CMP   r0, r1        @ Compara parámetro en r0 con cero
    BGE   end           @ Si r0 es >= saltar
    MVN   r4, r0        @ Copiar su valor negado (bit a bit) a r4
    ADD   r5, r4, #1    @ Sumar 1 para tener el complemnto a dos en r5
    MOV   r0, r5        @ Valor de retorno en r0
end:
    POP   {r4, r5}      @ Restaurar registros
    BX    lr            @ Retornar
