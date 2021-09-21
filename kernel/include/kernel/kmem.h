#ifndef INCLUDE_KERNEL_KMEM_H
#define INCLUDE_KERNEL_KMEM_H

//#define memsize 4294967296 // 4GB in bytes or
#define memsize 1048576 // in Pages
#define map_size memsize/8
#include <multiboot.h>
/*
typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;
*/ 
// this is really an entry, not the entire map.
typedef multiboot_memory_map_t mmap_entry_t;

void* get_physaddr(void*);
void map_page(void *physaddr, void *virtualaddr, unsigned int flags);
void mmap_init(void);
void mmap_set_allocated(unsigned int page);
void mmap_set_unallocated(unsigned int page);
int mmap_page_allocated(unsigned int page);
int mmap_byte_full(unsigned int page);
uint32_t* mmap_allocate_physical_page(void);
uint32_t* mmap_allocate_physical_pages(uint32_t* a,int num_pages);



#endif /* INCLUDE_KERNEL_KMEM_H */
