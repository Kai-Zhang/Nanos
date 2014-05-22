#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
Thread *PCB_of_thread_A, *PCB_of_thread_B, *PCB_of_thread_C, *PCB_of_thread_D;
void A () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("a");
            wakeup(PCB_of_thread_B);
            sleep();
        }
        x ++;
    }
}
void B () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("b");
            wakeup(PCB_of_thread_C);
            sleep();
        }
        x ++;
    }
}
void C () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("c");
            wakeup(PCB_of_thread_D);
            sleep();
        }
        x ++;
    }
}
void D () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printf("d");
            wakeup(PCB_of_thread_A);
            sleep();
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
	printf("printf test begin...\n");
printf("the answer should be:\n");
printf("#######################################################\n");
printf("Hello, welcome to OSlab! I'm the body of the game. ");
printf("Bootblock loads me to the memory position of 0x100000, and Makefile also tells me that I'm at the location of 0x100000. ");
printf("~!@#$^&*()_+`1234567890-=...... ");
printf("Now I will test your printf: ");
printf("1 + 1 = 2, 123 * 456 = 56088\n0, -1, -2147483648, -1412505855, -32768, 102030\n0, ffffffff, 80000000, abcdef01, ffff8000, 18e8e\n");
printf("#######################################################\n");
printf("your answer:\n");
printf("=======================================================\n");
printf("%s %s%scome %co%s", "Hello,", "", "wel", 't', " ");
printf("%c%c%c%c%c! ", 'O', 'S', 'l', 'a', 'b');
printf("I'm the %s of %s. %s 0x%x, %s 0x%x. ", "body", "the game", "Bootblock loads me to the memory position of", 
	0x100000, "and Makefile also tells me that I'm at the location of", 0x100000);
printf("~!@#$^&*()_+`1234567890-=...... ");
printf("Now I will test your printf: ");
printf("%d + %d = %d, %d * %d = %d\n", 1, 1, 1 + 1, 123, 456, 123 * 456);
printf("%d, %d, %d, %d, %d, %d\n", 0, 0xffffffff, 0x80000000, 0xabcedf01, -32768, 102030);
printf("%x, %x, %x, %x, %x, %x\n", 0, 0xffffffff, 0x80000000, 0xabcedf01, -32768, 102030);
printf("=======================================================\n");
printf("Test end!!! Good luck!!!\n");
assert(0);
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
