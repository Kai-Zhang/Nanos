#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
void thread_a (void) {
	int i = 0;
	for ( ; i < 100; ++i) {
		putchar('a');
	}
	return;
}
void thread_b (void) {
	while (1) {
		putchar('b');
	}
	assert(0);
}
void thread_c (void) {
	while (1) {
		putchar('c');
	}
	assert(0);
}
char string[] = "It all begins.";
void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_thread();
	printf("%s", string);
	enable_interrupt();
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
