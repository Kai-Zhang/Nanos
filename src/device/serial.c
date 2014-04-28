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
	char* print = NULL;
	int count = 0; unsigned int integer = 0;
	uint8_t a = 0;
	count = a;
	while (*str != 0) {
		if (*str != '%') {
			putchar (*str);
			++ str;
			++ count;
			continue;
		}
		switch (*(++str)) {
			case 'd':
				integer = *((int*)para);
				print = buf + sizeof(buf) - 2;
				do {
					*--print = '0' + integer % 10;
				} while (integer /= 10);
				for (; *print; ++ print) {
					putchar (*print);
				}
				para += 4; 
				break;
			case 'x':
				integer = *((unsigned int*)para);
				print = buf + sizeof(buf) - 2;
				do {
					*--print = (integer % 16 < 10) ?
								'0' + integer % 16 : 'a' + integer % 16 - 10;
				} while (integer >>= 4);
				for (; *print; ++ print) {
					putchar (*print);
				}
				para += 4;
				break;
			case 'c':
				putchar (*para);
				para += 4;
				break;
			case 's':
				print = *((char**)para);
				for (; *print; ++ print) {
					putchar (*print);
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
