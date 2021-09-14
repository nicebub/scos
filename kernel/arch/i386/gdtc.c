#include <stddef.h>
#include <stdint.h>
extern uint32_t gdttable[];
extern size_t gdts;

static struct gdtp {
    uint16_t limit;
    uint32_t f;
} __attribute ((packed)) gdpt;

extern void setGdt(struct gdtp*);

void setgdt(void) {
    gdpt.limit = gdts;
    gdpt.f = (uint32_t)&gdttable;
    setGdt(&gdpt);
}
