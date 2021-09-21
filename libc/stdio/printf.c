#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <kernel/klog.h>


enum num_type {
    UINT16 = 0,
    INT16,
    UINT32,
    INT32,
    HEX
};
static bool print(int device, const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (get_device(device).putc(bytes[i]) == EOF)
			return false;
	return true;
}
static bool dprintnum(int device, void* d, enum num_type type);

static bool printnum(void* d, enum num_type type) {
    return dprintnum(TTY, d, type);
}

static bool dprintnum(int device, void* d, enum num_type type) {
    char str[128], *p = &str[0];
    switch (type) {
        case UINT16:
        case INT16: {
            int16_t *data = (int16_t*)d;
            if (*data == 0) {
	            if (get_device(device).putc('0') == EOF) 
			        return false;
           }
           else {
            do{
                *p++ = (*data % 10) + '0';
                *data /= 10;
            } while(*data);
            p--;
            while(p != &str[0] - 1)
	            if (get_device(device).putc(*p--) == EOF) 
		    	    return false;
            }
        }
            break;
        case UINT32:
        case INT32:
        {
    int32_t *data = (int32_t*)d;
    if (*data == 0) {
	    if (get_device(device).putc('0') == EOF) 
			return false;
    }
    else {
        do{
         *p++ = (*data % 10) + '0';
         *data /= 10;
         } while(*data);
        while(p != &str[0] - 1)
	        if (get_device(device).putc(*--p) == EOF) 
		    	return false;
    }
    }
            break;
        case HEX: {
            uint32_t *data = (uint32_t*)d;
            if (*data == 0) {
	            if (get_device(device).putc('0') == EOF) 
			        return false;
	            if (get_device(device).putc('x') == EOF) 
			        return false;
        	    if (get_device(device).putc('0') == EOF) 
		        	return false;
            }
            else {
                do{
                    *p++ = (*data % 16) + '0';
                    *data /= 16;
                } while(*data);
                while(p != &str[0] - 1) {
                    if (*--p > '9') {
                        *p = ((*p - '0') - 10) + 'A';
                    }
        	        if (get_device(device).putc(*p) == EOF) {
        		    	return false;
        		    }
        		}
            }
          }
        break;
    }
	return true;
}
//int dvprintf(int device, const char* restrict format, va_list);
int dprintf(int device, const char* restrict format, ...){
    int i;
    va_list params;
    va_start(params, format);
    i = dvprintf(device, format, params);
    va_end(params);
    return i;
}
int printf(const char* restrict format, ...){
    int i;
    va_list params;
    va_start(params, format);
    i = dvprintf(TTY, format, params);
    va_end(params);
    return i;
}
int dvprintf(int device, const char* restrict format, va_list parameters) {
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(device, format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(device, &c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(device, str, len))
				return -1;
			written += len;
		} else if (*format == 'L') {
			format++;
			uint32_t d = (uint32_t) va_arg(parameters, uint32_t /* int promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!dprintnum(device, &d, UINT32))
				return -1;
			written++;
		} else if (*format == 'D') {
			format++;
			uint16_t d = (uint16_t) va_arg(parameters, int /* int promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!dprintnum(device, &d, UINT16))
				return -1;
			written++;
		} else if (*format == 'l') {
			format++;
			int32_t d = (int32_t) va_arg(parameters, int32_t /* int promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!dprintnum(device, &d, INT32))
				return -1;
			written++;
		} else if (*format == 'd') {
			format++;
			int16_t d = (int16_t) va_arg(parameters, int /* int promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!dprintnum(device, &d, INT16))
				return -1;
			written++;
		} else if (*format == 'x') {
			format++;
			int d = (int) va_arg(parameters, int /* int promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!dprintnum(device, &d, HEX))
				return -1;
			written++;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(device, format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	return written;
}

