#include <kernel/keyboard.h>


void take_keyboard_input(void) {
    volatile unsigned char c;
    c = get_last_keypress();
    if (c < 128) {
        c = decode_keypress(c);
        printf("%c", c);
    }
    else {
        decode_keypress(c);
    }
}

