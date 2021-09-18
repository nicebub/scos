#include <stdint.h>
#include <kernel/tty.h>

extern uint32_t system_timer_ms;

void ksystem_screen_uptime(void){
    uint32_t r = system_timer_ms;

    terminal_writestring_at_pos("system uptime:", 0, 46);
    if (((r / 36000) % 60) <= 9) {
        terminal_putnum_at_pos(0, 0, 60);
        terminal_putnum_at_pos((r / 36000) % 60, 0, 61);
    }
    else
        terminal_putnum_at_pos((r / 36000) % 60, 0, 60);
    terminal_writestring_at_pos(":", 0, 62);
    if (((r / 600) % 60) <= 9) {
        terminal_putnum_at_pos(0,0, 63);
        terminal_putnum_at_pos((r / 600) % 60, 0, 64);
    }
    else
        terminal_putnum_at_pos((r / 600) % 60, 0, 63);
/*
    terminal_writestring_at_pos(":", 0, 65);
    if (((r / 10) % 100) <= 9 ) {
        terminal_putnum_at_pos(0, 0, 66);
        terminal_putnum_at_pos(r / 10 % 100, 0, 67);
    }
    else
        terminal_putnum_at_pos(r / 10 % 100, 0, 66);
//	    ksleep(50);
*/
}

