#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/klog.h>
#include <kernel/serial.h>
extern uint32_t gdttable[];
extern size_t gdts;

static struct gdtp {
    uint16_t limit;
    uint32_t f;
} __attribute ((packed)) gdpt;

extern void setGdt(struct gdtp*);

void setgdt(void) {
    klog_all(KERN, "setting GDT limit to %d", gdts);
/*    terminal_putd(gdts);
    serial_putd(gdts);*/
//    klog_all(KERN, "\n");
    gdpt.limit = gdts;
    klog_all(KERN, "setting GDT address to %d", (uint32_t)&gdttable);
/*    terminal_putd((uint32_t)&gdttable);
    serial_putd((uint32_t)&gdttable);*/
//    klog_all(KERN, "\n");
    gdpt.f = (uint32_t)&gdttable;
/*    terminal_putc('\n');
    serial_putc('\n');*/
    klog_all(KERN, "finalize setting GDT...");
    setGdt(&gdpt);
}

