# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VMODE,    1<<2             # enable video
.set FLAGS,    ALIGN | MEMINFO |VMODE # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002      # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0 # Set to 0 to enable framebuffer by default, 1 to disable
.long 640 # Number of horizontal pixels
.long 480 # Number of vertical pixels
.long 32 # Bit depth

# Note that these values do not DICTATE what the bootloader will give us. 
# It will only tell the bootloader what we PREFER.

# PAGE TABLE SETUP
.section .bss, "aw", @nobits
    .align 4096        # make sure we are aligned on the 4 KiB page mark
boot_page_directory:
    .skip 4096         # skip 4 KB for a Page Directory
boot_page_table1:      
    .skip 4096         # skip 4 KB for the 1st Page Table
    

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom: # stack grows downwards on x86
    .skip 16384 # 16 KiB stack size
stack_top:

# The kernel entry point....sort of still adjusting bootstrap
.section .multiboot.text, "a"
.global _start
.extern init_devices
.type _start, @function
.align 4
_start:
    
#get current or actual page table address - 1st page
    movl $(boot_page_table1 - 0xC0000000), %edi

#start mapping at address 0x0
    movl $0, %esi

#store for later use for remapping VGA framebuffer
    movl $1023, %ecx

1:
#    cmpl $_kernel_start, %esi
#    jl 2f

# /if we found the end of the kernel we are done remapping
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f
    movl %esi, %edx
# permissions adjusting per section
# /if we are at the beginning of a .text section
    cmpl $_text_begin, %esi
    jl q
# /else if we are at the end of a .text section
    cmpl $_text_end, %esi
    jge q
# /else we are at the beginning of a .rodata section
    cmpl $_rodata_begin, %esi
    jl q
# /else we are at the end of a .rodata section
    cmpl $_rodata_end, %esi
    jge q
# otherwise set to read-only
# read-only/present permission
    orl $0x001, %edx
    jmp s
q:
# read/write/present permission
    orl $0x003, %edx
s:
# store the information on the page into the page table
    movl %edx, (%edi)
2:
# move to the next page and next memory address to read
    addl $4096, %esi
    addl $4, %edi
    loop 1b
    
3:

# loop done

# 0x000B8000 VGA video framebuffer address remap to
# read/write/present permissions and reset to virtual address 0xC03FF000
    movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

# remap page table from current address to current address + 0xc0000000 Identity Page
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
# remap page table from current address to
# current address + 0xc0000000 +0x300000 3M mark
# read/write permissions, present
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

    movl $(boot_page_directory - 0xc0000000 + 0x003), boot_page_directory - 0xc0000000 + 1023 * 4


# move the current address of the page directory into place in the CR3 reg
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

# turn paging and page guard on in the CR0 reg    
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

# jump
# to get the CPU to reload the proper page info    
    lea 4f, %ecx
# jump as stated
    jmp *%ecx


.section .text
4:

   #PAGING is now turned ON

# remove Identity page by putting 0 in its place
    movl $0, boot_page_directory + 0

# reload TLB?
    movl %cr3, %ecx
    movl %ecx, %cr3

   # lower memory boot or kernel? has been turned off
   # only access through high memory now    

   #initialize stack pointer
initStack:
    mov $stack_top, %esp
# put magic value on stack for main function

    push %eax
# put value of currently GRUB given Memory onto stack as well,
# but since grub didn't know what we were doing with virtual memory
# we have to adjust for our tables
    add $0xc0000000, %ebx
    push %ebx
    call init_devices
    #initialize terminal and buffer to start outputting messages
    # to VGA buffer currently if can
   call terminal_initialize
    # log some initial messages
   call klog_call1

   #setup GDT
    cld # may be a System V ABI std to cld before function entry?
    call setgdt # C function to help setup the Global Descriptor Table

reloadSegments:
    # far jump to reload segment
    # and start using the GDT
    ljmp  $0x08,$reload_CS   # 0x08 is the CS for the kernel
reload_CS:

    #another few logs
    
   call klog_call2

   # NOW USING GDT AND ITS SEGMENTS AND ACCESS
   # RESTRICTIONS, I THINK
   # reset the selectors

    mov $0x10, %ax # 0x10 was the selector we chose for the data selector in the table
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

    mov %esp, %esp

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
#.size _start, . - _start
#
#.section .rodata
#.p2align 4
#.global GDT_START
#GDT_START:
#.ascii "GDT set\n\0"
