#ifndef INCLUDE_KERNEL_KLOG_H
#define INCLUDE_KERNEL_KLOG_H

#include <kernel/devices.h>

static volatile const char* levels[] = {
    "DEBUG",
    "ERROR",
    "INFO",
    "USER",
    "SYSTEM",
    "KERN",
    NULL
};

enum loglevel{
    DEBUG = 0,
    ERROR,
    INFO,
    USER,
    SYSTEM,
    KERN,
};
int klog_putc(int c);
int klog_putd(int c);
int klog_puts(const char * s);

int klog(int level, int device, const char* fmt, ...);
void write_to_log(const char* level, const char* ftm, ...);
void klog_all(int level, const char* fmt, ...);

#endif /* INCLUDE_KERNEL_KLOG_H */
