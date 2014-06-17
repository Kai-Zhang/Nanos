#ifndef __THREAD_H__
#define __THREAD_H__
#include "x86/memory.h"
#include "x86/cpu.h"
#include "common/list.h"
#include "common/types.h"

#define STK_SZ 2048
#define THREAD_NUM 66

struct Thread {
	struct TrapFrame* tf;
	char kstack[STK_SZ];
	struct list_head runq, freeq, semq;
	pid_t pid;
	int is_sleeping;
	int lock_counter;
};
typedef struct Thread Thread;

extern Thread thread_pool[THREAD_NUM];
extern Thread *current, *free, *sleeping, *next;

static inline void
schedule(void) {
	if (current != thread_pool || !list_empty(&(current->runq))) {
		current = list_entry(current->runq.next, Thread, runq);
	}
	if (current == thread_pool && !list_empty(&(current->runq))) {
		current = list_entry(current->runq.next, Thread, runq);
	}
}

void init_thread(void);
Thread* create_kthread(void (*entry)(void));
void sleep(void);
void wakeup(Thread* t);
void stop_thread(void);

static inline void
lock(void) {
	disable_interrupt();
	++ current->lock_counter;
}

static inline void
unlock(void) {
	-- current->lock_counter;
	if (current->lock_counter <= 0) {
		enable_interrupt();
	}
}

#endif

