#ifndef __SERIAL_H__
#define __SERIAL_H__

/* 保证向下兼容，支持Lab1中的测试数据 */
#define printk printf
void init_serial(void);
void putchar(char);
int printk(const char*, ...);

#define SERIAL_PORT  0x3F8

#endif
