#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
static Thread *a, *b;
void thread_a (void) {
	lock();
	putchar('a');
	sleep();
	while (1) {
		putchar('a');
	}
	assert(0);
}
void thread_b (void) {
	int i = 0;
	for ( ; i < 10; ++i) {
		putchar('b');
	}
	wakeup(a);
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
void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_thread();
	enable_interrupt();
	a = create_kthread(thread_a);
	b = create_kthread(thread_b);
	create_kthread(thread_c);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
