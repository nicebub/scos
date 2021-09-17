.extern CountDown
.section .text
.global sleep
.type sleep, @function
sleep:
    push %ebp
    mov %esp, %ebp
    push %eax
    mov 8(%ebp), %eax
    mov %eax, (CountDown)
SleepLoop:
    cli
    mov (CountDown), %eax
    or %eax, %eax
    jz SleepDone
    sti
    nop
    nop
    nop
    nop
    nop
    nop
    jmp SleepLoop
SleepDone:
    sti
    pop %eax
    pop %ebp
    ret

