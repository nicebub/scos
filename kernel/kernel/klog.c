#include <kernel/serial.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* levels[] = {
    "DEBUG",
    "ERROR",
    "INFO",
    "USER",
    "SYSTEM",
    "KERN"
};

enum loglevel{
    DEBUG = 0,
    ERROR,
    INFO,
    USER,
    SYSTEM,
    KERN
};

enum device {
    SERIAL = 0,
    TTY,
    LOG
};

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
void klog_call1(void) {
    for(int i = 0; i < 3; i++) {
        klog(KERN, "16 KiB stack", i);
        klog(KERN, "Page Directory 4 KiB", i);
        klog(KERN, "Pages 4 KiB", i);
        klog(KERN, "Paging enable", i);
    }
}

