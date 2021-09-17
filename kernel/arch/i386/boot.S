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
   #PAGING is now turned ON 
    movl $0, boot_page_directory + 0
    movl %cr3, %ecx
    movl %ecx, %cr3

   # lower memory boot or kernel? has been turned off
   # only access through high memory now    

   #initialize stack pointer
.extern klog
    mov $stack_top, %esp
   call terminal_initialize
   call klog_call1
   #setup GDT
    cld
    call setgdt

reloadSegments:
    # far jump to reload segment
    # and start using the GDT
    ljmp  $0x08,$reload_CS
reload_CS:

   call klog_call2

   # NOW USING GDT AND ITS SEGMENTS AND ACCESS
   # RESTRICTIONS, I THINK

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # reinitialize the stack
    # not sure its needed
    # but incase accessing
    # the stack selector will be
    # different after GDT is set

    mov $stack_top, %esp
	# Call the global constructors.
 	 call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
#.size _start, . - _start

.section .rodata
.p2align 4
.global GDT_START
GDT_START:
.ascii "GDT set\n\0"

.section .data
.p2align 4
.global KERNEL_LOG
.global KERNEL_LOG_END
KERNEL_LOG:
    .skip 16384 # 16 KiB log size
KERNEL_LOG_END:
