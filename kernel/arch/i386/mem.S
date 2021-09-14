.section .text
.extern _terminal_writestring
.global detect_low_mem
.type detect_low_mem, @function

S:
.ascii "string is this"
R:
.ascii "Error: could not run int 0x12"
detect_low_mem:
#    sti
#    push S
    movl S, %edx
#    movl S, %esi
#    call terminal_writestring
    clc
   int $0x12
    jc .Error
    mov $0x86, %ah
    mov $0xff, %cx
    mov $0x4240, %dx
    int $0x15
#    cli
    ret
.Error:
    movl S, %edx
#    call terminal_writestring
    ret

