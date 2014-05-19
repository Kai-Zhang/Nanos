#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
static Thread *a, *b;
long long volatile counter = 0;
void thread_a (void) {
//	while (1) {
//		lock();
//		lock();
//		putchar('a');
//		unlock();
//		sleep();
//		putchar('s');
//		unlock();
//		putchar('a');
//		sleep();
//		wakeup(b);
//	}
	int i = 0;
	while (1) {
		lock();
		lock();
		putchar('a');
		unlock();
		for (i = 0; i < 1000; ++i) {
			asm volatile ("nop");
		}
		sleep();
	}
}
void thread_b (void) {
	int i = 0;
	while (1) {
		for (i = 0; i < 100; ++i) {
			putchar('b');
		}
		wakeup(a);
	//	putchar('b');
	//	wakeup(a);
//		putchar('b');
//		wakeup(a);
//		sleep();
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
//	create_kthread(thread_c);
	enable_interrupt();
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
