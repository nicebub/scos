/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 0x00100000;
 
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	_kernel_start = .;
	.multiboot.data : {
	    *(.multiboot.data)
	}
	
	.multiboot.text : {
	    *(.multiboot.text)
	}
	
	. += 0xC0000000;
	
	.text ALIGN(4K) : AT (ADDR (.text) - 0xC0000000)
	{
    _text_begin = .;
		*(.text)
		*(.init)
		*(.fini)
    _text_end = .;
	}
	/* Read-only data. */
	.rodata ALIGN(4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
    _rodata_begin = .;
		*(.rodata)
    _rodata_end = .;
	}
 
	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT (ADDR (.data) - 0xC0000000)
	{
    _data_begin = .;
		*(.data)
    _data_end = .;
	}
	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT (ADDR (.bss) - 0xC0000000)
	{
    _bss_begin = .;
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
    _bss_end = .;
	}
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
    _kernel_end = .;
}
