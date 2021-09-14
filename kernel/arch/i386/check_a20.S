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
    
    mov byte (%ds:%si), %al
    push %ax

    mov $0x00, byte (%es:%di)
    mov $0xFF, byte (%ds:%si)
    
    cmp $0xFF, byte (%es:%di)
    
    pop %ax
    mov %al, byte (%ds:%si)
    
    pop %ax
    mov %al, byte (%es:%di)
    
    mov 0, %ax 
    je check_a20__exit
    
    mov 1, %ax
    
 check_a20__exit:
    ret
