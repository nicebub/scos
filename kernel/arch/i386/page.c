#include <stdint.h>
#include <string.h>
#include <kernel/klog.h>
#include <kernel/kmem.h>

static inline void __native_flush_tlb_single(unsigned long addr) {
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
static inline volatile uint32_t* __get_page_directory() {
 uint32_t ret;
   __asm__ __volatile__ (
        "movl %%cr3, %%eax\n\t"
        "movl %%eax, %0\n\t"
        : "=m" (ret)
        : 
        : "%eax");
   return (uint32_t*)ret + 0xc0000000;
}

void *get_physaddr(void *virtualaddr) {
    volatile unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    volatile unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    volatile unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.

    volatile unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
    __native_flush_tlb_single(pt[ptindex]);
}


static volatile uint8_t mmap[map_size] = {1};
static volatile unsigned int mmap_last_alloc = 0;
void mmap_init(void){
    memset((void*)mmap, 1, sizeof mmap);
}

void mmap_set_allocated(unsigned int page){
    mmap[page/8] &= ~(1<<(page % 8));
    mmap_last_alloc = page;
}

void mmap_set_unallocated(unsigned int page){
    mmap[page/8] |= (1<<(page % 8));    
}

int mmap_page_allocated(unsigned int page){
    return !(mmap[page/8] & ~(1<<(page %8)));
}

int mmap_byte_full(unsigned int page){
    return (mmap[page/8] == 0);
}
static inline uint32_t* mmap_search_loop(unsigned int start, unsigned int finish) {
    for (unsigned int i = start; i < finish; i++) {
        if ( mmap_byte_full(i)) {
            i += 8;
            continue;
        }
        if (!mmap_page_allocated(i)) {
            mmap_set_allocated(i);
            return (uint32_t*)(i << 12);
        }
    }
    return NULL;
}

uint32_t* mmap_allocate_physical_page(void) {
    uint32_t* r;
    r = mmap_search_loop(mmap_last_alloc+1, memsize);
    if (r) return r;
    r = mmap_search_loop(0, mmap_last_alloc);
    if (r) return r;
    // failed to find a unallocated page
    return NULL;    
}

uint32_t* mmap_allocate_physical_pages(uint32_t* a,int num_pages){
    int p = num_pages;
    memset(a, 0, sizeof(*a)*num_pages);
    while (p){
        if (!(a[num_pages - p--] = (uint32_t)mmap_allocate_physical_page()))
            goto err;
    }
    return a;
err:
    klog_all(KERN, "couldn't map all the physical pages requested");
    return a;
}

