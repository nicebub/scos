#include <kernel/serial.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <kernel/klog.h>

#define LOG_SIZE 4096
static char KERNEL_LOG[LOG_SIZE];
static char * log = KERNEL_LOG;

int klog_putc(int c){
    *log++ = c;
    if (log == KERNEL_LOG[LOG_SIZE-1])
        log = (char*)KERNEL_LOG;
    
}
int klog_putd(int c){
/* write */
}
int klog_puts(const char * s){
    for (size_t i =0; i < strlen(s); i++)
        klog_putc(s[i]);
}


void write_to_log(const char* level, const char* str, ...){
    va_list args;
    va_start(args, str);
    if ((&KERNEL_LOG[LOG_SIZE] - log) < (strlen(str) + strlen(level) + 13))
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
    va_end(args);
}
int klog(int level, int dev, const char* str, ...) {
    va_list args;
    dprintf(dev, "Log level %s:", levels[level]);
/*    dprintf(dev, levels[level]);
    dprintf(dev, ": ");*/

    va_start(args, str);
    dvprintf(dev, str, args);
    va_end(args);

    get_device(dev).putc('\n');
    
    return 1;
}
void klog_all(int level, const char* str, ...) {
    va_list args;
    va_start(args, str);
    for(int i = 0; i < 3; i++)
        klog(level, i, str, args);
    va_end(args);

}
void klog_call1(void) {
        klog(KERN, TTY, "16 KiB stack");
        klog(KERN, TTY, "Virtual Memory Page Directory 4 KiB");
        klog(KERN, TTY, "Virtual Memory Pages 4 KiB");
        klog(KERN, TTY, "Paging enabled");
        klog(KERN, SERIAL, "16 KiB stack");
        klog(KERN, SERIAL, "Virtual Memory Page Directory 4 KiB");
        klog(KERN, SERIAL, "Virtual Memory Pages 4 KiB");
        klog(KERN, SERIAL, "Paging enabled");
}
void klog_call2(void) {
        klog(KERN, TTY, "Long jmp from GDT use success!");
        klog(KERN, SERIAL, "Long jmp from GDT use success!");
}
void klog_call3(int freq) {
    klog(KERN, TTY, "Timer Frequency: %D", 1193182/freq);
    klog(KERN, SERIAL, "Timer Frequency: %D", 1193182/freq);
/*    serial_putnum(1193182/freq);
    serial_putchar('\n');
    terminal_putnum(1193182/freq);
    terminal_putchar('\n');
    */
}
