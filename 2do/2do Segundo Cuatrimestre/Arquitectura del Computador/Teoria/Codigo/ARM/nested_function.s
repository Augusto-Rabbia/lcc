/*
   Main program calls plusTwo(5) and places result in r6
*/
.text
.global main
main:
   @save any r0-r3 I need... (none)
   MOV   r0, #5      @load 5 as
   BL    plusTwo
   MOV   r6, r0      @get my return value, store into r6
   @restore any r0-r3... (none)

   @stop program
   MOV r7, #1
   SWI 0

@-------------------------------------------------------------
plusTwo:
   @@@ Prologue ----------------------------------------
   PUSH  {lr}              @store my return address

   @@@ Body --------------------------------------------
   @r0 already has the parameter to pass
   BL   plusOne
   @r0 has x + 1
   BL   plusOne
   @r0 has x + 2

   @@@ Epilog --------------------------------------------
   POP   {lr}                   @Restore my return address and                 @saved registers
   BX    lr                     @retur (return value is in r0)
@----------------------------------------------------------------------


@----------------------------------------------------------------------
plusOne:
   @@@ Prologue ----------------------------------------
   PUSH  {lr}              @store my return address

   @@@ Body --------------------------------------------
   ADD   r0, r0, #1
   @r0 has x + 1

   @@@ Epilog --------------------------------------------
   POP   {lr}              @Restore my return address and saved registers
   BX    lr                @return (return value is in r0)
@----------------------------------------------------------------------