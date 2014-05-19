#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
static Thread *a, *b;
long long volatile counter = 0;
void thread_c(void);
void thread_a (void) {
	int i = 0;
	for ( ; i < 1000; ++i) {
		putchar('a');
	}
	return;
}
void thread_b (void) {
	int i = 0;
	while (++i) {
		putchar('b');
		if (i == 10000)
			create_kthread(thread_c);
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
	enable_interrupt();
	a = create_kthread(thread_a);
	b = create_kthread(thread_b);
//	create_kthread(thread_c);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
