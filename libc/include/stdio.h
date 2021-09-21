#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
 
#define EOF (-1)
 
#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char* __restrict, ...);
int dprintf(int device, const char* __restrict, ...);
int putc(int);
int dputc(int dev, int);
int puts(const char*);
int dputs(int dev, const char *);

#ifdef __cplusplus
}
#endif
 
#endif

