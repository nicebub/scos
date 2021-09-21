#ifndef INCLUDE_KERNEL_RTC_H
#define INCLUDE_KERNEL_RTC_H

uint8_t rtc_read(uint8_t index);
void rtc_write(uint8_t index, uint8_t val);
int get_CMOS(void);

#endif /* INCLUDE_KERNEL_RTC_H */
