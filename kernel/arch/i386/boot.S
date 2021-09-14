# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss, "aw", @nobits
    .align 4096
boot_page_directory:
    .skip 4096
boot_page_table1:
    .skip 4096

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
    .skip 16384 # 16 KiB stack size
stack_top:


# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
.align 4
_start:
    movl $(boot_page_table1 - 0xC0000000), %edi
    movl $0, %esi
    movl $1023, %ecx

1:
    cmpl $_kernel_start, %esi
    jl 2f
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f
    movl %esi, %edx
    cmpl $_text_begin, %esi
    jl q
    cmpl $_text_end, %esi
    jge q
    cmpl $_rodata_begin, %esi
    jl q
    cmpl $_rodata_end, %esi
    jge q
    orl $0x001, %edx
    jmp s
q:
    orl $0x003, %edx
s:
    movl %edx, (%edi)
2:
    addl $4096, %esi
    addl $4, %edi
    loop 1b
    
3:
    movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
    
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4
    
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3
    
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0
    
    lea 4f, %ecx
    jmp *%ecx

.section .text
4:
    movl $0, boot_page_directory + 0
    
    movl %cr3, %ecx
    movl %ecx, %cr3
    
    mov $stack_top, %esp
    mov $stack_top, %ebp

    cld
    call setgdt

reloadSegments:
    ljmp  $0x08,$reload_CS
reload_CS:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

	# Call the global constructors.
    mov $stack_top, %esp
    mov $stack_top, %ebp
 	 call _init
#    cld
#    call terminal_initialize
    cld
    call idt_init
    cld
    call pic_check
    cld
    call detect_low_mem
	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
#.size _start, . - _start
