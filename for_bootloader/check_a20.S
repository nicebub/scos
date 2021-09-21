.section .text
.global check_a20
check_a20:
    cli
    xor %ax, %ax
    mov %ax, %es
    not %ax
    mov %ax, %ds
    mov $0x0500, %di
    mov $0x0510, %si
    lea (%es:%di), %al
    mov (%al), %al
    push %ax
    
    movb (%ds:%si), %al
    push %ax

    movb $0x00, (%es:%di)
    movb $0xFF, (%ds:%si)
    
    cmpb $0xFF, (%es:%di)
    
    pop %ax
    movb %al, (%ds:%si)
    
    pop %ax
    movb %al, (%es:%di)
    
    mov 0, %ax 
    je check_a20__exit
    
    mov 1, %ax
    
 check_a20__exit:
    ret
