#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>
void terminal_remap(void);
void terminal_noremap_init(void);
void terminal_initialize(void);
int terminal_putc(int c);
int terminal_putd(int c);
int terminal_puts(const char* s);
void terminal_write(const char* data, size_t size);
void terminal_write_at_pos(const char* data, size_t s, int x, int y);
void terminal_writestring_at_pos(const char* data, int x, int y);
void terminal_putnum_at_pos(int d, int x, int y);

#endif

