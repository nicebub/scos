#ifndef INCLUDE_KERNEL_SERIAL_H
#define INCLUDE_KERNEL_SERIAL_H
#include <stddef.h>

int serial_putc(int a);
int serial_putd(int d);
void serial_write(const char* data, size_t size);
int serial_puts(const char* data);

#endif

