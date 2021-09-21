#ifndef INCLUDE_KERNEL_PIT_H
#define INCLUDE_KERNEL_PIT_H

extern uint32_t  IRQ0_fractions;
extern uint32_t  IRQ0_ms;
extern uint32_t  IRQ0_frequency;
extern uint32_t  PIT_reload_value;
extern uint32_t  system_timer_fractions;
extern uint32_t  system_timer_ms;
extern void init_PIT(uint32_t);

#endif /* INCLUDE_KERNEL_PIT_H */
