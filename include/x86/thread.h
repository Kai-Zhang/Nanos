#ifndef __THREAD_H__
#define __THREAD_H__
#include "x86/memory.h"
#include "x86/cpu.h"
#include "x86/sem.h"
#include "x86/message.h"
#include "common/list.h"
#include "common/types.h"

#define	STK_SZ		1024
#define	MSGBUF_SZ	256
#define	THREAD_NUM	66

struct Thread {
	/* Stack for a thread */
	struct TrapFrame* tf;
	char kstack[STK_SZ];
	/* Link node for thread management */
	struct list_head runq, freeq, semq;
	/* Thread's status */
	pid_t pid;
	int is_sleeping;
	int lock_counter;
	int unlock_status;
	/* Message */
	Semaphore mutex, amount;
	Message* messages;
	Message msg_buf[MSGBUF_SZ];
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
	asm volatile ("pushf");
	asm volatile ("popl %0" : "=r" (current->unlock_status));
	current->unlock_status = (current->unlock_status & 0x200) >> 9;
	disable_interrupt();
	++ current->lock_counter;
}

static inline void
unlock(void) {
	-- current->lock_counter;
	if (current->lock_counter <= 0) {
		if (current->unlock_status) {
			enable_interrupt();
		}
	}
}

#endif

