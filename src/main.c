#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);

Thread* a = NULL;

void A(void) {
	int i = 0;
	while (1) {
		if (i % 100000 == 0) {
			putchar('a');
			if (i == 200000) {
				sleep();
			}
		}
		++ i;
	}
}

void B(void) {
	int i = 0;
	while (1) {
		if (i % 10000 == 0) {
			putchar('b');
			if (i == 300000) {
				wakeup(a);
			}
		}
		++ i;
	}
}
void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_thread();
	enable_interrupt();
	create_kthread(grading);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	a = create_kthread(A);
	create_kthread(B);
	while (1) {
		wait_for_interrupt();
	}
}
