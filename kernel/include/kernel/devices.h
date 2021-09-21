#ifndef INCLUDE_KERNEL_DEVICES_H
#define INCLUDE_KERNEL_DEVICES_H
#include <stddef.h>

#define NUM_DEVS 3
static volatile const char* device_strs[] = {
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

typedef struct kdevs {
    enum device dev;
    int (*putc)(int c);
    int (*putd)(int c);
    int (*puts)(const char * s);
} kdev_t;


void init_devices(void);
kdev_t get_device(int d);
#endif /* INCLUDE_KERNEL_DEVICES_H */
