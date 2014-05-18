#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
static Thread *a, *b;
void thread_a (void) {
	while (1) {
		putchar('a');
		sleep();
		wakeup(b);
	}
}
void thread_b (void) {
	while (1) {
		putchar('b');
		wakeup(a);
		sleep();
	}
}
void thread_c (void) {
	while (1) {
		putchar('c');
	}
}
void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_thread();
//	asm volatile ("movl %0, %%esp" :
//		: "a" (&(thread_stack[0].kstack[STK_SZ]))
//		: "%esp");
	a = create_kthread(thread_a);
	b = create_kthread(thread_b);
	create_kthread(thread_c);
	enable_interrupt();
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
