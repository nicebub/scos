#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>

extern uint32_t gdttable[];
extern size_t gdts;

static struct gdtp {
    uint16_t limit;
    uint32_t f;
} __attribute ((packed)) gdpt;

extern void setGdt(struct gdtp*);

void setgdt(void) {
    terminal_writestring("setting GDT limit to ");
    terminal_putnum(gdts);
    terminal_putchar('\n');
    gdpt.limit = gdts;
    terminal_writestring("setting GDT address to ");
    terminal_putnum((uint32_t)&gdttable);
    terminal_putchar('\n');
    gdpt.f = (uint32_t)&gdttable;
    terminal_writestring("finalize setting GDT\n");
    setGdt(&gdpt);
    terminal_writestring("GDT finalized\n");
}

