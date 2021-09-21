#include <kernel/devices.h>
#include <kernel/serial.h>
#include <kernel/klog.h>
#include <kernel/tty.h>

static volatile kdev_t devices[NUM_DEVS] = {0};

void init_devices(void) {
    devices[SERIAL].dev = SERIAL;
    devices[TTY].dev = TTY;
    devices[LOG].dev = LOG;
    devices[SERIAL].putc = serial_putc;
    devices[TTY].putc = terminal_putc;
    devices[LOG].putc = klog_putc;
}

kdev_t get_device(int d) {
    return devices[d];
}
