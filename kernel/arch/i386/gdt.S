.section .text
.p2align 4
.global setGdt
.global gdttable
.global gdts
.type setGdt, @function
setGdt:
    push %eax
    push $disablestring
    push $0x05
    call klog_all
    pop %eax
    pop %eax
    pop %eax
    cli
    push $loadstring
    push $0x05
    call klog_all
    pop %eax
    pop %eax
    mov 4(%esp), %eax
    lgdt (%eax)
    ret

.section .data
.p2align 4
gdttable:
  .word 0x0000, 0x0000
  .byte 0, 0, 0, 0
  .word 0xffff, 0x0000
  .byte 0, 0x9a, 0xcf, 0
  .word 0xffff, 0x0000
  .byte 0, 0x92, 0xcf, 0
gdts:
.word (. - gdttable)

.section .rodata
.p2align 4
disablestring:
.ascii "temporarily disabling interrupts\0"
loadstring:
.ascii "loading GDT\0"
