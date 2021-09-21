#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/klog.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>

extern uint32_t gdttable[];
extern size_t gdts;

static gdtp_t gdpt;

void setgdt(void) {
    klog_all(KERN, "setting GDT limit to %D", gdts);
/*    terminal_putd(gdts);
    serial_putd(gdts);*/
//    klog_all(KERN, "\n");
    gdpt.limit = gdts;
    klog_all(KERN, "setting GDT address to %x", (uint32_t)&gdttable);
/*    terminal_putd((uint32_t)&gdttable);
    serial_putd((uint32_t)&gdttable);*/
//    klog_all(KERN, "\n");
    gdpt.f = (uint32_t)&gdttable;
/*    terminal_putc('\n');
    serial_putc('\n');*/
    klog_all(KERN, "finalize setting GDT...");
    setGdt(&gdpt);
}

