.section .text
.p2align 4
.extern _terminal_writestring
.global detect_low_mem
.type detect_low_mem, @function

.ascii "Error: could not run int 0x12"
detect_low_mem:
    clc
   int $0x12
    jc .Error
    mov $0x86, %ah
    mov $0xff, %cx
    mov $0x4240, %dx
    int $0x15
    ret
.Error:
    ret

