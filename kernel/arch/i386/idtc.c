#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/klog.h>
#include <kernel/serial.h>
#include <kernel/interrupts.h>
__attribute__((aligned(0x10)))
static idtp_t idpt;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

extern void* isr_stub_table[]; 


uint32_t exception_handler(registers_t* regs){
//   terminal_writestring("Made it inside exception handler\n");
    switch (regs->int_no)
    {
            
      default:
            break;
    }
    klog_all(KERN, "uncaught exception: %x", regs->int_no);
/*    serial_putd(regs->int_no);
    serial_putc('\n');
    terminal_putd(regs->int_no);
    terminal_putc('\n');*/
//    __asm__ volatile("cli; hlt");
}



void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attrib     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}



void idt_init() {
    klog_all(KERN, "IDT Address: %x", (int)(unsigned int)&idt[0]);
  
/*    terminal_putd((int)(unsigned int)&idt[0]);
    terminal_putc('\n');
    serial_putd((int)(unsigned int)&idt[0]);
    serial_putc('\n');*/
    idpt.f = (uintptr_t)&idt[0];
    idpt.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
//        idt_set_descriptor(0, isr_stub_table[0], 0x0E);
    for (uint8_t vector = 0; vector < 48; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
//        vectors[vector] = true;
    }
}
void turn_on_interrupts(void) {
    klog_all(KERN, "Enabling Interrupts..");
    __asm__ volatile ("lidt %0" : : "m"(idpt)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}
void turn_of_interrupts(void) {
    klog_all(KERN, "Disabling Interrupts.."); 
    __asm__ volatile ("cli"); // set the interrupt flag
}
void panic(const char * s){
    klog_all(KERN,"%s", s);
    stack_dump();
}

#define MAX_NESTED_EXCEPTIONS 3
void gpfExcHandler(void) {
    static volatile int nestexc = 0;
   if (nestexc > MAX_NESTED_EXCEPTIONS) panic("too many nested exceptions!");
   nestexc++;
    klog_all(KERN, "nesting exceptions!!!");
/*
 
   if (!fix_the_error()) {
     write_an_error_message();
   }*/
   nestexc--;
   return;
}
void dump_hex(char* stack){
    while(stack)
        terminal_putc(*stack--);
}

