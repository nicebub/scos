#include <stdlib.h>
#include <kernel/tty.h>
#include "io.h"

#define PORT 0x3f8          // COM1
 
int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
//   outb(PORT + 1, 0x01);    // Enable all interrupts

   return 0;
}

int serial_received() {
   return inb(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT);
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void serial_putchar(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT,a);
}
void serial_putnum(int d) {
    char str[128], *p = &str[0];
    if (d == 0) {
	    serial_putchar('0');
    }
    else {
            do{
                *p++ = (d % 10) + '0';
                d /= 10;
            } while(d);
            p--;
            while(p != &str[0] - 1)
	           serial_putchar(*p--);
    }
}

void serial_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		serial_putchar(data[i]);
}
 
void serial_writestring(const char* data) {
	serial_write(data, strlen(data));
}
/*
void terminal_write_at_pos(const char* data, size_t s, int x, int y){
    for (size_t i = 0; i < s; i++)
    	terminal_putentryat(data[i], terminal_color, y+i, x);
}
void terminal_writestring_at_pos(const char* data, int x, int y){
    terminal_write_at_pos(data, strlen(data), x, y);
}
*/
