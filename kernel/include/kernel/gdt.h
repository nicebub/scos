#ifndef INCLUDE_KERNEL_GDT_H
#define INCLUDE_KERNEL_GDT_H

#include <stdint.h>

typedef struct gdtp {
    uint16_t limit;
    uint32_t f;
} __attribute ((packed)) gdtp_t;

extern void setGdt(struct gdtp*);

const char disablestring[] = "temporarily disabling interrupts";
const char loadstring[] = "loading GDT";
#endif /* INCLUDE_KERNEL_GDT_H */
