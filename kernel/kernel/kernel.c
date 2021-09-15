#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/pic.h>
extern void init_PIT(void);
extern int init_serial(void);
void kernel_main(void) {
//    terminal_initialize();
    idt_init();
    pic_check();
    init_PIT();
    init_serial();
    detect_low_mem();
//    printf("%d\n", check_a20);    
    set_keyboard_pic_mask();
    int x = 0;
	while (x++ < 20)
	   printf("Hello, kernel World!\n");

}

