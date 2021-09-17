#ifndef INCLUDE_KERNEL_TIMER_H
#define INCLUDE_KERNEL_TIMER_H
static struct TimerBlock {
//    EXCHANGE e;
    uint32_t CountDown;

} timerblocks[20];

void init_timerblocks(void);
struct TimerBlock* findTimerBlock(void);

#endif /* INCLUDE_KERNEL_TIMER_H */
