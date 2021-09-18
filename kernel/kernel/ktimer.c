#include <stddef.h>
#include <stdint.h>
#include <kernel/ktimer.h>
#include <kernel/klog.h>

#define COUNTDOWN_DONE_MSG 1

volatile static uint32_t timersused = 0;
volatile static struct TimerBlock timerblocks[20];
 
void init_timerblocks(void) {
    for (int i = 0; i < 20; i++) {
        timerblocks[i].CountDown = 0;
    }
}

int timerset(struct TimerBlock* t) {
    for (int i = 0; i < 20; i++) {
        if (t == &timerblocks[i]) {
            return timersused & (1<<i);
        }
    }
}
void TimerIRQ(void) /* called from Assembly */
{
    uint8_t i;
 
    for (i = 0; i < 20; i++) {
//        klog_all(KERN, "checking counters");
        if (timerblocks[i].CountDown > 0) {
//            klog_all(KERN,"decrementing countdown of timer");
            timerblocks[i].CountDown--;
            if (timerblocks[i].CountDown == 0) {
//                printf("timer done\n");
                
//                SendMessage(timerblocks[i].e, COUNTDOWN_DONE_MESSAGE);
                timersused &=  ~(1<<i);
                  
            }
        }
    }
//        klog_all(KERN, "done checking timers");
}

    
struct TimerBlock* findTimerBlock(void) {
    for (int i =0; i < 20; i++) {
        if (!(timersused & (1<<i))) {
            timersused |=  (1<<i);
//            printf("timersused is now %d\n", timersused);
  //          klog_all(KERN, "found timer"); 
            return &timerblocks[i];
        }
    }
//    klog_all(KERN, "no open timer found");    
    return NULL;
}

void releaseTimerBlock(struct TimerBlock * t){
    for (int i = 0; i < 20; i++) {
        if (t == &timerblocks[i]) {
            timerblocks[i].CountDown = 0;
            timersused &= ~(1<<i);
        }
    }
}

