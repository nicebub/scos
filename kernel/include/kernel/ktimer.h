#ifndef INCLUDE_KERNEL_TIMER_H
#define INCLUDE_KERNEL_TIMER_H

struct TimerBlock {
//    EXCHANGE e;
    uint32_t CountDown;

};

void init_timerblocks(void);
struct TimerBlock* findTimerBlock(void);
void releaseTimerBlock(struct TimerBlock * t);

#endif /* INCLUDE_KERNEL_TIMER_H */
