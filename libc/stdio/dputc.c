#include <stdio.h>
 
#if defined(__is_libk)
#include <kernel/tty.h>
#include <kernel/devices.h>
#endif
 
int dputc(int dev, int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	get_device(dev).putc(ic);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}

