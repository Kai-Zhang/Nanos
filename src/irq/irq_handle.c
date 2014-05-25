#include "x86.h"
#include "device.h"
#include "common.h"

void
irq_handle(struct TrapFrame *tf) {
	if (tf->irq == 1000) {
		current->tf = tf;
		schedule();
		next = current;
	} else if (tf->irq == 1001) {
		current->tf = tf;
		uint32_t code = in_byte(0x60);
		uint32_t val = in_byte(0x61);
		out_byte(0x61, val | 0x80);
		out_byte(0x61, val);
		putchar('0' + code / 100);
		putchar('0' + code / 10 % 10);
		putchar('0' + code % 10);
		putchar('\n');
	} else if (tf->irq == 0x80) {
		current->tf = tf;
		current = next;
	} else {
		assert(0);
	}
}

