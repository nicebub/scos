#ifndef INCLUDE_KERNEL_BOOT_H
#define INCLUDE_KERNEL_BOOT_H
#include <stdint.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _text_begin;
extern uint32_t _text_end;
extern uint32_t _rodata_begin;
extern uint32_t _rodata_end;
extern uint32_t _data_begin;
extern uint32_t _data_end;
extern uint32_t _bss_begin;
extern uint32_t _bss_end;
extern uint32_t boot_page_directory;
extern uint32_t boot_page_table1;
extern uint32_t stack_bottom;
extern uint32_t stack_top;
extern const char GDT_START[9];

#endif /*INCLUDE_KERNEL_BOOT_H */
