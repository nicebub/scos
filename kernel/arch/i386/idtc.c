#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>

typedef struct regss {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) registers_t;

typedef struct idtent {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t reserved;
    uint8_t attrib;
    uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

static struct idtp {
    uint16_t limit;
    uint32_t f;
} __attribute__ ((packed)) idpt;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];


uint32_t exception_handler(registers_t*);
uint32_t exception_handler(registers_t* regs){
//   terminal_writestring("Made it inside exception handler\n");
    switch (regs->int_no)
    {
            
      default:
            break;
    }
    terminal_writestring("uncaught exception: ");
    terminal_putnum(regs->int_no);
    terminal_putchar('\n');
//    __asm__ volatile("cli; hlt");
}


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attrib     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

extern void* isr_stub_table[]; 

void idt_init(void);

void idt_init() {
    terminal_writestring("IDT ADDRESS: ");
    terminal_putnum(&idt[0]);
    terminal_putchar('\n');
    idpt.f = (uintptr_t)&idt[0];
    idpt.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
//        idt_set_descriptor(0, isr_stub_table[0], 0x0E);
    for (uint8_t vector = 0; vector < 48; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
//        vectors[vector] = true;
    }
    terminal_writestring("Turning on Interrupts.."); 
    __asm__ volatile ("lidt %0" : : "m"(idpt)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
    terminal_writestring("done\n");
}

