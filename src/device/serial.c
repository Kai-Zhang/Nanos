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
	uint8_t* para = (uint8_t*)(&format + 1);
	char* print = NULL;
	uint32_t integer = 0;
	int count = 0;

	while (*str != 0) {
		if (*str != '%') {
			putchar (*str);
			++ str;
			++ count;
			continue;
		}
		switch (*(++str)) {
			case 'd':
				integer = *((uint32_t*)para);
				if ((int)integer < 0) {
					putchar ('-');
					integer = integer ^ 0x80000000;
				}
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
				integer = *((uint32_t*)para);
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
