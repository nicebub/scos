#ifndef INCLUDE_KERNEL_KLOG_H
#define INCLUDE_KERNEL_KLOG_H

static const char* levels[] = {
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
static const char* devices[] = {
    "SERIAL",
    "TTY",
    "LOG",
    NULL
};

enum device {
    SERIAL = 0,
    TTY,
    LOG,
};

int klog(int level, const char* str, int device );

#endif /* INCLUDE_KERNEL_KLOG_H */
