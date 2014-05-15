#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);

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
	while (1);
}
