#include <stddef.h>
#include <stdint.h>
#include <kernel/ktimer.h>

#define COUNTDOWN_DONE_MSG 1

static int timersused = 0;

 
void init_timerblocks(void) {
    for (int i = 0; i < 20; i++) {
        timerblocks[i].CountDown = 0;
    }
}
void TimerIRQ(void) /* called from Assembly */
{
    uint8_t i;
 
    for (i = 0; i < 20; i++)
        if (timerblocks[i].CountDown > 0) {
            timerblocks[i].CountDown--;
            if (timerblocks[i].CountDown == 0) {
//                SendMessage(timerblocks[i].e, COUNTDOWN_DONE_MESSAGE);
                timersused = timersused & ~(1<<i);
                  
            }
        }
}

    
struct TimerBlock* findTimerBlock(void) {
    for (int i =0; i < 20; i++) {
        if (!(timersused & (1<<i))) {
            timersused = timersused | (1<<i);
            return &timerblocks[i];
        }
    }
    return NULL;
}

