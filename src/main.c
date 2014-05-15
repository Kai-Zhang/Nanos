#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
void thread_a (void) {
	while (1) {
		putchar('a');
	}
}
void thread_b (void) {
	while (1) {
		putchar('b');
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
	create_kthread(thread_a);
	create_kthread(thread_b);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
