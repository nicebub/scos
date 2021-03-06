#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "io.h"
#include <kernel/keyboard.h>

static volatile unsigned char kbbuf[KBUF_SZ];
static volatile unsigned char* kbbp = &kbbuf[0];
static volatile unsigned char KEYFLAGS;

enum kb_flags {
    CONTROL,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    ALT,
    CAPS,
    NUM_LOCK,
    SCROLL_LOCK,
    
};

static const unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void handle_kb(void) {
    volatile unsigned char c;
    c = inb(0x60);
//    printf("keyboard test: keycode: %x actual: %c\n", c, kbdus[c]);
    *kbbp++ = c;
    if (kbbp > &kbbuf[KBUF_SZ-1])
        kbbp = &kbbuf[0];

    return;
}
void init_keyboard_buffer() {
    memset(kbbuf, -1, sizeof kbbuf);
}

unsigned char decode_keypress(unsigned char c) {
    return kbdus[c];
}


unsigned char get_last_keypress(void) {
    unsigned char r = *kbbp;
    *kbbp = -1;

    if (kbbp == &kbbuf[KBUF_SZ-1]) {
        kbbp = &kbbuf[0];
    }
    else {
        kbbp++;
    }
    return r;
}
