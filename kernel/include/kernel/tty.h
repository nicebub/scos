#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>
 
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_putnum(int c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_write_at_pos(const char* data, size_t s, int x, int y);
void terminal_writestring_at_pos(const char* data, int x, int y);
void terminal_putnum_at_pos(int d, int x, int y);

#endif

