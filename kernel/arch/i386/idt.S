.macro isr_err_stub in
isr_stub_\in:
    cli
    push $\in
    jmp isr_common
.endm

.macro isr_no_err_stub in
isr_stub_\in:
    cli
    push $0x00
    push $\in
    jmp isr_common
.endm

.section .text
.extern exception_handler
isr_common:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    movl %esp, %eax
    push %eax
    cld
    call exception_handler
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds

    popal

    add $8, %esp

    sti
    iret


.section .text
.extern handle_kb
.extern PIT_reload_value
.extern IRQ0_ms
.extern IRQ0_fractions
.extern IRQ0_frequency
.extern system_timer_ms
.extern system_timer_fractions
.extern gpfExcHandler
.extern TimerIRQ
.extern dump_hex
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
exc_0d_handler:
    push %gs
    mov 0x00, %gs
    push %eax
    mov %gs:0xb8000, %eax
    movw $'D', (%eax)
    # D in the top-left corner means we're handling
    #  a GPF exception right ATM.
 
    # your 'normal' handler comes here
    pushal
    push %ds
    push %es
    mov 0x10, %ax
    mov %ax, %ds
    mov %ax, %es
 
    call gpfExcHandler
 
    pop %es
    pop %ds
    popal
    mov %gs:0xb8000, %eax
    movw $'D', (%eax)
    # the 'D' moved one character to the right, letting
    # us know that the exception has been handled properly
    # and that normal operations continues.
    pop %eax
    pop %gs
    iret
isr_14_handler:
    cli
    push $14
    cli
    hlt
    iret
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
IRQ0_handler:
    cli
    pushal
    push %eax
    push %ebx
    
    mov (IRQ0_fractions), %eax
    mov (IRQ0_ms), %ebx
    addl %eax, (system_timer_fractions)
    adc %ebx, (system_timer_ms)
    push %ds
    push %es
    push %fs
    push %gs
    mov %esp, %eax
    push %eax
    cld
    call TimerIRQ
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    mov $0x20, %al
    out %al, $0x20
    pop %ebx
    pop %eax
    popal
    sti
    iret

keyboard_handler:
    cli
    pushal
    call handle_kb
    mov $0x20, %al
    out %al, $0x20

    popal
    sti
    iret
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47

.global isr_stub_table
isr_stub_table:
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long exc_0d_handler
    .long isr_14_handler
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31
    .long IRQ0_handler
    .long keyboard_handler
    .long isr_stub_34
    .long isr_stub_35
    .long isr_stub_36
    .long isr_stub_37
    .long isr_stub_38
    .long isr_stub_39
    .long isr_stub_40
    .long isr_stub_41
    .long isr_stub_42
    .long isr_stub_43
    .long isr_stub_44
    .long isr_stub_45
    .long isr_stub_46
    .long isr_stub_47

.global stack_dump
stack_dump:
  push %ebp
  mov %esp, %ebp
  call dump_hex
  pop %ebp
  ret
