#include <stdio.h>
#include <stdint.h>
#include "io.h"

void check_keyboard(void);


void check_keyboard(void) {
    unsigned char c;
    printf("checking keyboard\n");
    c = inb(0x60);
    switch (c) {
    
    }
}
