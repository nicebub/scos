#ifndef INCLUDE_KERNEL_PIC_H
#define INCLUDE_KERNEL_PIC_H
#include <stdint.h>

void PIC_sendEOI(unsigned char irq);
void PIC_remap(int offset1, int offset2);
void IRQ_set_mask(unsigned char IRQline);
void IRQ_clear_mask(unsigned char IRQline);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);
void pic_check(void);
void set_keyboard_pic_mask(void);


#endif /*INCLUDE_KERNEL_PIC_H */
