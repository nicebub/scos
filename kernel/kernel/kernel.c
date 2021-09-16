#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/pic.h>
extern void init_PIT(void);
extern int init_serial(void);
extern uint16_t system_timer_fractions;
extern uint16_t system_timer_ms;

void print_uptime(void);

void kernel_main(void) {
//    terminal_initialize();
    idt_init();
    pic_check();
    init_PIT();
    init_serial();
    detect_low_mem();
//    printf("%d\n", check_a20);    
    set_keyboard_pic_mask();
//    int x = system_timer_fractions + 1000;
	while (1){
	    print_uptime();
	}
//	    while (system_timer_fractions < x);
//	    x = system_timer_fractions + 1000;
//	   printf("Hello, kernel World!\n");

}

void print_uptime(){
    int x = system_timer_fractions, r = system_timer_ms;
//    int res = x;
    terminal_writestring_at_pos("system uptime:", 0, 56);
    terminal_putnum_at_pos(x, 0, 70);
}
