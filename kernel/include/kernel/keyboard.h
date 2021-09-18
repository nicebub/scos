#ifndef INCLUDE_KERNEL_KEYBOARD_H
#define INCLUDE_KERNEL_KEYBOARD_H
#define KBUF_SZ 128

unsigned char decode_keypress(unsigned char c);
void init_keyboard_buffer(void);
unsigned char decode_keypress(unsigned char c);
unsigned char get_last_keypress(void);
void take_keyboard_input(void);

#endif /* INCLUDE_KERNEL_KEYBOARD_H */
