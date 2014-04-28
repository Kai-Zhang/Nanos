#include "x86.h"
#include "device.h"

void
init_serial(void) {
	out_byte(SERIAL_PORT + 1, 0x00);
	out_byte(SERIAL_PORT + 3, 0x80);
	out_byte(SERIAL_PORT + 0, 0x01);
	out_byte(SERIAL_PORT + 1, 0x00);
	out_byte(SERIAL_PORT + 3, 0x03);
	out_byte(SERIAL_PORT + 2, 0xC7);
	out_byte(SERIAL_PORT + 4, 0x0B);
}

static inline int
serial_idle(void) {
	return (in_byte(SERIAL_PORT + 5) & 0x20) != 0;
}

void
putchar(char ch) {
	while (serial_idle() != TRUE);
	out_byte(SERIAL_PORT, ch);
}

int printf (const char* format, ...) {
	static char buf [30] = {0};
	const char* str = format;
	char* para = (char*)(&format + 1);
	char* prt = NULL;
	int count = 0; int dec = 0;

	while (*str != 0) {
		if (*str != '%') {
			putchar (*str);
			++ str;
			++ count;
			continue;
		}
		switch (*(++str)) {
			case 'd':
				dec = *((int*)para);
				prt = buf + sizeof(buf) - 2;
				do {
					*--prt = '0' + dec % 10;
				} while (dec /= 10);
				for (; *prt; ++ prt) {
					putchar (*prt);
				}
				para += 4; 
				break;
			case 'c':
				putchar (*para);
				para += 4;
				break;
			case 's':
				prt = *((char**)para);
				for (; *prt; ++ prt) {
					putchar (*prt);
				}
				para += 4;
				break;
			default:
				putchar (*str);
		}
		++ str;
		++ count;
	}
	return count;
}
