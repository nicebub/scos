#ifndef INCLUDE_KERNEL_SERIAL_H
#define INCLUDE_KERNEL_SERIAL_H
#include <stddef.h>

void serial_putchar(char a);
void serial_putnum(int d);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);

#endif

