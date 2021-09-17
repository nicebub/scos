.section .data
.global CountDown
CountDown: .double 0

#.section .text
#.global TimerIRQ
#TimerIRQ:
#    push %eax
#    mov (CountDown), %eax
#    or %eax, %eax
#    jz TimerDone
#    mov (CountDown), %eax
#    dec %eax
#    mov %eax, (CountDown)
#TimerDone:
#    pop %eax
#    iret


