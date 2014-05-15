#ifndef __THREAD_H__
#define __THREAD_H__
#include "x86/memory.h"

#define STK_SZ 8192
#define THREAD_NUM 10

struct Thread {
	struct TrapFrame* tf;
	char kstack[STK_SZ];
	struct list_head runq, freeq;
};
typedef struct Thread Thread;

extern Thread thread_stack[THREAD_NUM];
extern Thread *current, *next, *sleeping;
extern volatile int lock_counter;

void init_thread(void);
Thread* create_kthread(void (*entry)(void));
void sleep(void);
void wakeup(Thread* t);
void lock(void);
void unlock(void);

#endif

