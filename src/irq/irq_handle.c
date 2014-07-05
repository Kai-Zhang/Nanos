#include "x86.h"
#include "device.h"
#include "common.h"

volatile int is_irq = 0;

void
irq_handle(struct TrapFrame *tf) {
	is_irq = 1;
	current->tf = tf;
	if (tf->irq == 1000) {
		schedule();
		next = current;
//		update_sched();
//		update_jiffy();
//		send_updatemsg();
	} else if (tf->irq == 1001) {
//		send_keymsg();
		uint32_t code = in_byte(0x60);
		uint32_t val = in_byte(0x61);
		out_byte(0x61, val | 0x80);
		out_byte(0x61, val);
		putchar('0' + code / 100);
		putchar('0' + code / 10 % 10);
		putchar('0' + code % 10);
		putchar('\n');
	} else if (tf->irq == 0x80) {
		current = next;
	} else {
		assert(0);
	}
	is_irq = 0;
}

