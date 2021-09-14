# 0 "arch/i386/idt.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "arch/i386/idt.S"
.macro isr_err_stub in
isr_stub_\in:
    cli
# push $0x00
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
# add $8, %esp
    movl %esp, %eax
    push %eax
# cld
    call exception_handler
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds

    popal

    add $8, %esp

# push $8
# cld
# call PIC_sendEOI
# pop %eax
# push $7
# cld
# call PIC_sendEOI
# pop %eax

    sti
    iret

.extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub 8
isr_no_err_stub 9
isr_err_stub 10
isr_err_stub 11
isr_err_stub 12
isr_err_stub 13
isr_err_stub 14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub 17
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
isr_err_stub 30
isr_no_err_stub 31


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
    .long isr_stub_13
    .long isr_stub_14
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

.section .text
.p2align 4
.global setIdt
.global idttable
.global idts
.type setIdt, @function

setIdt:
    cli
    mov 4(%esp), %eax
    lidt (%eax)
    ret


.section .data

idttable:
  .word 0x0000, 0x0000
  .byte 0, 0, 0, 0
  .word 0xffff, 0x0000
  .byte 0, 0x9a, 0xcf, 0
  .word 0xffff, 0x0000
  .byte 0, 0x92, 0xcf, 0
idts:
.word (. - idttable - 1)
.long idttable
