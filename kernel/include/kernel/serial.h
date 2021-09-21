#ifndef INCLUDE_KERNEL_SERIAL_H
#define INCLUDE_KERNEL_SERIAL_H

#include <stddef.h>

int init_serial(void);
int serial_putc(int a);
int serial_putd(int d);
void serial_write(const char* data, size_t size);
int serial_puts(const char* data);
int is_transmit_empty(void);
char read_serial(void);
int serial_received(void);

#endif

