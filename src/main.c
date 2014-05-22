#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
Thread *PCB_of_thread_A, *PCB_of_thread_B, *PCB_of_thread_C, *PCB_of_thread_D;
void A () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
			//lock();
            printf("a");
            wakeup(PCB_of_thread_B);
            sleep();
			//unlock();
        }
        x ++;
    }
}
void B () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
			//lock();
            printf("b");
            wakeup(PCB_of_thread_C);
            sleep();
			//unlock();
        }
        x ++;
    }
}
void C () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
			//lock();
            printf("c");
            wakeup(PCB_of_thread_D);
            sleep();
			//unlock();
        }
        x ++;
    }
}
void D () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
			//lock();
            printf("d");
            wakeup(PCB_of_thread_A);
            sleep();
			//unlock();
        }
        x ++;
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
	PCB_of_thread_A = create_kthread(A);
	PCB_of_thread_B = create_kthread(B);
	PCB_of_thread_C = create_kthread(C);
	PCB_of_thread_D = create_kthread(D);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}


void
grading (void) {
	
}
