#include <kernel/serial.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/klog.h>
extern char KERNEL_LOG[];
extern void *  KERNEL_LOG_END;

void write_to_log(const char* level, const char* str){

    static char * log = NULL;
    if (log == NULL || (long unsigned int)((char*)KERNEL_LOG_END  - log - 1) <= (strlen(str) + strlen(level) + 13))
        log = (char*)KERNEL_LOG;
    memcpy(log, "LOG LEVEL ", 10);
    log += 10;
    memcpy(log, level, strlen(level));
    log += strlen(level);
    *log++ = ':';
    *log++ = ' ';
    memcpy(log, str, strlen(str));
    log += strlen(str);
    *log++ = '\n';
}
int klog(int level, const char* str, int dev) {
    switch (dev) {
        case SERIAL:
            serial_writestring("LOG LEVEL ");
            serial_writestring(levels[level]);
            serial_writestring(": ");
            serial_writestring(str);
            serial_putchar('\n');
            break;
        case TTY:
            terminal_writestring("LOG LEVEL ");
            terminal_writestring(levels[level]);
            terminal_writestring(": ");
            terminal_writestring(str);
            terminal_putchar('\n');
            break;
        case LOG:
            write_to_log(levels[level], str);
            break;
    }
    return 1;
}
void klog_all(int level, const char* str) {
    for(int i = 0; i < 3; i++)
        klog(level, str, i);

}
void klog_call1(void) {
        klog_all(KERN, "16 KiB stack");
        klog_all(KERN, "Page Directory 4 KiB");
        klog_all(KERN, "Pages 4 KiB");
        klog_all(KERN, "Paging enable");
}
void klog_call2(void) {
        klog_all(KERN, "long jmp from GDT use success!");
}
void klog_call3(int freq) {
    klog_all(KERN, "Timer Frequency: ");
    serial_putnum(1193182/freq);
    serial_putchar('\n');
    terminal_putnum(1193182/freq);
    terminal_putchar('\n');
}
