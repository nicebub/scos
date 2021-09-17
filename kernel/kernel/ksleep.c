#include <stdint.h>
#include <stddef.h>
#include <kernel/ksleep.h>
#include <kernel/ktimer.h>

void ksleep(uint32_t delay)
{
    struct TimerBlock *t;
 
    if ((t = findTimerBlock()) == NULL)
        return;
    t->CountDown = delay;
 //   WaitForMessageFrom(t->e = getCrntExch());
}
