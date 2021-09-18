#include <stdint.h>
#include <stddef.h>
#include <kernel/ksleep.h>
#include <kernel/ktimer.h>

void ksleep(uint32_t delay)
{
    volatile struct TimerBlock *t;
 
    if ((t = findTimerBlock()) == NULL)
        return;
     t->CountDown = delay;
     while (1) {
//        printf("timer: %d\n", t->CountDown);
        if (t->CountDown == 0)
            break;
        t;
     }
//     releaseTimerBlock(t);
//    printf("sleep done\n");
    return;
}
