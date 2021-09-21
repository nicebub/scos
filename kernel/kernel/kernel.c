#include <kernel/tty.h>
#include <kernel/pic.h>
#include <kernel/ktime.h>
#include <kernel/ksleep.h>
#include <multiboot.h>
#include <kernel/kmem.h>
#include <stdio.h>
#include <kernel/keyboard.h>
#include <kernel/ktimer.h>
#include <kernel/klog.h>

extern  int init_serial(void);
extern void init_PIT(uint32_t);
extern void turn_on_interrupts(void);
extern void idt_init(void);
extern void pic_check(void);
extern void detect_high_mem_e820(void);
extern void set_keyboard_pic_mask(void);
extern void panic(const char*);
extern void* get_physaddr(void*);
extern void _init(void);

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
 
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }
 
    /* Loop through the memory map and display the values */
    int i;
    for(i = 0; i < mbd->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = 
            (multiboot_memory_map_t*) (mbd->mmap_addr + i + 0xc0000000);
 
        klog_all(KERN, "Start Addr High:Low:|Length High:Low:|Size:|Type:");
        klog_all(KERN, "%x:%x   %x:%x   %x  %x",
            mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
    }	mmap_entry_t* entry = mbd->mmap_addr + 0xc0000000;
	while(entry < mbd->mmap_addr + mbd->mmap_length + 0xc0000000) {
		// do something with the entry
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
	_init();
    idt_init();
//    detect_high_mem_e820();
//    memreport_one();
//    detect_high_mem_e881();
//    detect_high_mem_e801();
//    get_CMOS();
    turn_on_interrupts();
    pic_check();
    /* largest divider for slowest timer? */

    init_timerblocks();
    init_PIT(9500);
    get_physaddr(0xc010b8f4);
    klog_all(KERN, "TESTING PRINT FUNCTIONS");
    klog_all(KERN, "%x", 8675309);
    init_serial();
    init_keyboard_buffer();
    set_keyboard_pic_mask();
	while (1){
//	    ksleep(10);
        take_keyboard_input();
	    ksystem_screen_uptime();
	}
}

