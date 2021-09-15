#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
 
#include <kernel/tty.h>
 
#include "vga.h"
#define ROWS 25
#define COLS 80

static const size_t VGA_WIDTH = COLS;
static const size_t VGA_HEIGHT = ROWS;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000;
 
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer, frame[ROWS][COLS];

void terminal_swap_buffer(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = frame[y][x];
		}
	}
}
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) VGA_MEMORY;
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			frame[y][x] = vga_entry(' ', terminal_color);
		}
	}
	terminal_swap_buffer();
}
 
void terminal_scroll_down(void) 
{
	for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			frame[y][x] = frame[y+1][x];
		}
	}
    for (size_t x = 0; x < VGA_WIDTH; x++) {
		frame[VGA_HEIGHT-1][x] = vga_entry(' ', terminal_color);
	}
	terminal_swap_buffer();
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	frame[y][x] = vga_entry(c, color);
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_scroll(int line) {
	size_t *loop;
	char c;
 
	for(loop = (size_t*)(line * (VGA_WIDTH * 2) + VGA_MEMORY); (size_t)loop < VGA_WIDTH * 2; loop++) {
		c = *loop;
		*(loop - (VGA_WIDTH * 2)) = c;
	}
}
 
void terminal_delete_last_line() {
	size_t x;
	int *ptr;
 
	for(x = 0; x < VGA_WIDTH * 2; x++) {
		ptr = (int*)(VGA_MEMORY + (VGA_WIDTH * 2) * (VGA_HEIGHT - 1) + x);
		*ptr = 0;
	}
}
 
void terminal_putchar(char c) {
	size_t line;
/*	unsigned char uc = c;*/
 
    if (c == '\n') {
        terminal_row++;
        terminal_column = -1;
    }
    else {
    	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}
	if (terminal_row == VGA_HEIGHT)
	{
	    terminal_scroll_down();
		terminal_row = VGA_HEIGHT - 1;
	}
}
void terminal_putnum(int d) {
    char str[128], *p = &str[0];
    if (d == 0) {
	    terminal_putchar('0');
    }
    else {
            do{
                *p++ = (d % 10) + '0';
                d /= 10;
            } while(d);
            p--;
            while(p != &str[0] - 1)
	           terminal_putchar(*p--);
    }
}
 
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

