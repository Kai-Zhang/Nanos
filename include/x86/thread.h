#ifndef __THREAD_H__
#define __THREAD_H__
#include "x86/memory.h"
#include "common/list.h"

#define STK_SZ 2048
#define THREAD_NUM 65

struct Thread {
	struct TrapFrame* tf;
	char kstack[STK_SZ];
	struct list_head runq, freeq;
	int is_sleeping;
};
typedef struct Thread Thread;

extern Thread thread_stack[THREAD_NUM];
extern Thread *current, *running, *next, *sleeping;
extern volatile int lock_counter;

static inline void
schedule(void) {
	if (current != thread_stack || !list_empty(&(current->runq))) {
		current = list_entry(current->runq.next, Thread, runq);
	}
	if (current == thread_stack && !list_empty(&(current->runq))) {
		current = list_entry(current->runq.next, Thread, runq);
	}
}

void init_thread(void);
Thread* create_kthread(void (*entry)(void));
void sleep(void);
void wakeup(Thread* t);
void lock(void);
void unlock(void);

#endif

