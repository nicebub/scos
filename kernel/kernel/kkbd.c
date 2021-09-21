#include <kernel/keyboard.h>
#include <kernel/klog.h>

void take_keyboard_input(void) {
    volatile unsigned char c;
    c = get_last_keypress();
    if (c < 128) {
        c = decode_keypress(c);
        klog_all(KERN, "%c", c);
    }
    else {
        decode_keypress(c);
    }
}

