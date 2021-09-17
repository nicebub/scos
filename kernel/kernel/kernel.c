#include <kernel/tty.h>
#include <kernel/pic.h>
#include <kernel/ktime.h>
#include <kernel/ksleep.h>

extern  int init_serial(void);
extern void init_PIT(uint32_t);
extern void idt_init(void);
extern void pic_check(void);
extern void detect_low_mem(void);
extern void set_keyboard_pic_mask(void);

void kernel_main(void) {
    idt_init();
    pic_check();
    /* largest divider for slowest timer? */
    init_PIT(1193);
    init_serial();
    detect_low_mem();
    set_keyboard_pic_mask();

	while (1){
	    ksleep(54);
	    ksystem_screen_uptime();
	}
}

