#include <kernel/io.h>
#include <kernel/rtc.h>
#include <stdint.h>

#define PORT_CMOS_INDEX 0x0070
#define PORT_CMOS_DATA 0x0071
#define NMI_DISABLE_BIT 0x8
uint8_t rtc_read(uint8_t index)
{
    index |= NMI_DISABLE_BIT;
    outb(index, PORT_CMOS_INDEX);
    return inb(PORT_CMOS_DATA);
}
    
void rtc_write(uint8_t index, uint8_t val)
{
    index |= NMI_DISABLE_BIT;
    outb(index, PORT_CMOS_INDEX);
    outb(val, PORT_CMOS_DATA);
}


int get_CMOS(void)
{
	uint32_t memory_size;

	memory_size = (rtc_read(0x34) | (rtc_read(0x35) << 8)) * 64 * 1024;
	printf("Memory Size: %x\n", memory_size);
    printf("0M-1M: %x\n", (rtc_read(0x15) | (rtc_read(0x16) << 10)) );
    printf("1M-16M: %x\n", (rtc_read(0x18) | (rtc_read(0x17) << 10)) );
    printf("16M-4G: %x\n", (rtc_read(0x31) | (rtc_read(0x30) << 16)) );
	return 0;
}



