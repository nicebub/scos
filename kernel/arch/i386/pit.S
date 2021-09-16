 #Input
 # ebx   Desired PIT frequency in Hz
 .section .text
 .global init_PIT
init_PIT:
    pushal
 
    # Do some checking
 
    movl $0x10000, %eax                 #eax = reload value for slowest possible frequency (65536)
    cmp $18 , %ebx                      #Is the requested frequency too low?
    jbe .gotReloadValue                 # yes, use slowest possible frequency
 
    mov $1, %eax                         #ax = reload value for fastest possible frequency (1)
    cmp $1193181, %ebx                   #Is the requested frequency too high?
    jae .gotReloadValue               # yes, use fastest possible frequency
 
    # Calculate the reload value
 
    mov $3579545, %eax
    mov $0, %edx                         #edx:eax = 3579545
    div %ebx                           #eax = 3579545 / frequency, edx = remainder
    push %eax
    push %ebx
    push %edx
    mov $3579545, %eax
    mov $2, %ebx
    mov $0, %edx
    div %ebx
    mov %eax, %ecx
    pop %edx
    pop %ebx
    pop %eax
    cmp %ecx, %edx               #Is the remainder more than half?
    jb .l1                            # no, round down
    inc %eax                           # yes, round up
 .l1:
    mov $3, %ebx
    mov $0, %edx                         #edx:eax = 3579545 * 256 / frequency
    div %ebx                           #eax = (3579545 * 256 / 3 * 256) / frequency
    push %eax
    push %ebx
    push %edx
    mov $3, %eax
    mov $2, %ebx
    mov $0, %edx
    div %ebx
    mov %eax, %ecx
    pop %edx
    pop %ebx
    pop %eax
    cmp %ecx, %edx                     #Is the remainder more than half?
    jb .l2                            # no, round down
    inc %eax                           # yes, round up
 .l2:
 
 
 # Store the reload value and calculate the actual frequency
 .extern PIT_reload_value
 .extern IRQ0_ms
 .extern IRQ0_fractions
 .extern IRQ0_frequency
 .gotReloadValue:
    push %eax                          #Store reload_value for later
    mov  %ax, (PIT_reload_value)         #Store the reload value for later
    mov %ax, %bx                       #ebx = reload value
 
    mov $3579545, %eax 
    mov $0, %edx                         #edx:eax = 3579545
    div %ebx                           #eax = 3579545 / reload_value, edx = remainder
    push %eax
    push %ebx
    push %edx
    mov $3579545, %eax
    mov $2, %ebx
    mov $0, %edx
    div %ebx
    mov %eax, %ecx
    pop %edx
    pop %ebx
    pop %eax
    cmp %ecx, %edx               #Is the remainder more than half?
    jb .l3                            # no, round down
    inc %eax                           # yes, round up
 .l3:
    mov $3, %ebx
    mov $0, %edx                         #edx:eax = 3579545 / reload_value
    div %ebx                           #eax = (3579545 / 3) / frequency
    push %eax
    push %ebx
    push %edx
    mov $3, %eax
    mov $2, %ebx
    mov $0, %edx
    div %ebx
    mov %eax, %ecx
    pop %edx
    pop %ebx
    pop %eax
    cmp %ecx, %edx                     #Is the remainder more than half?
    jb .l4                            # no, round down
    inc %eax                           # yes, round up
 .l4:
    mov %eax, (IRQ0_frequency)          #Store the actual frequency for displaying later
 
 
 # Calculate the amount of time between IRQs in 32.32 fixed point
 #
 # Note: The basic formula is:
 #           time in ms = reload_value / (3579545 / 3) * 1000
 #       This can be rearranged in the following way:
 #           time in ms = reload_value * 3000 / 3579545
 #           time in ms = reload_value * 3000 / 3579545 * (2^42)/(2^42)
 #           time in ms = reload_value * 3000 * (2^42) / 3579545 / (2^42)
 #           time in ms * 2^32 = reload_value * 3000 * (2^42) / 3579545 / (2^42) * (2^32)
 #           time in ms * 2^32 = reload_value * 3000 * (2^42) / 3579545 / (2^10)
 
    pop %ebx                           #ebx = reload_value
    mov $0xDBB3A062, %eax                #eax = 3000 * (2^42) / 3579545
    mul %ebx                           #edx:eax = reload_value * 3000 * (2^42) / 3579545
    shrd $10, %edx, %eax
    shr $10, %edx                        #edx:eax = reload_value * 3000 * (2^42) / 3579545 / (2^10)
 
    mov %edx, (IRQ0_ms)                 #Set whole ms between IRQs
    mov %eax, (IRQ0_fractions)          #Set fractions of 1 ms between IRQs
 
 
 # Program the PIT channel
 
    pushf
    cli                               #Disabled interrupts (just in case)
 
    mov $0x34, %al                   #channel 0, lobyte/hibyte, rate generator
    out %al, $0x43
 
    mov (PIT_reload_value), %ax         #ax = 16 bit reload value
    out %al, $0x40                       #Set low byte of PIT reload value
    mov %ah, %al                         #ax = high 8 bits of reload value
    out %al, $0x40                       #Set high byte of PIT reload value
 
    popf
 
    popal
    ret
