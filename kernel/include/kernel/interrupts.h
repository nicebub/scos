#ifndef INCLUDE_KERNEL_INTERRUPTS_H
#define INCLUDE_KERNEL_INTERRUPTS_H

#include <stdint.h>

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

typedef struct idtp {
    uint16_t limit;
    uint32_t f;
} __attribute__ ((packed)) idtp_t;


extern void stack_dump(void);

void turn_on_interrupts(void);
void turn_of_interrupts(void);
uint32_t exception_handler(registers_t*);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);
void panic(const char *);
void dump_hex(char* stack);
void gpfExcHandler(void);

#endif /* INCLUDE_KERNEL_INTERRUPTS_H */
