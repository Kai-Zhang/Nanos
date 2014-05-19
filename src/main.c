#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
static Thread *a, *b;
long long volatile counter = 0;
void thread_a (void) {
	int i=0;
	for ( ;i < 1000000000; ++i) {
		++counter;
	}
	printf(  "%d  ", counter);
	while (1) {
		wait_for_interrupt();
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
	}
}
void thread_b (void) {
	int j=0;
	for ( ;j < 1000000000; ++j) {
		++counter;
	}
	printf("  %d  ", counter);
	while (1) {
		wait_for_interrupt();
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
	//create_kthread(thread_c);
	enable_interrupt();
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
