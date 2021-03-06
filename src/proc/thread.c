#include "x86.h"
#include "common.h"
#include "device.h"

Thread thread_pool[THREAD_NUM];
Thread *current, *free, *sleeping, *next;
volatile int lock_counter;

void
init_thread(void) {
	current = &thread_pool[0];
	INIT_LIST_HEAD(&(thread_pool[0].runq));
	INIT_LIST_HEAD(&(thread_pool[0].freeq));
	thread_pool[0].pid = 0;
	thread_pool[0].is_sleeping = 0;
	thread_pool[0].lock_counter = 0;
	thread_pool[0].unlock_status = 0;

	free = &thread_pool[1];
	INIT_LIST_HEAD(&(thread_pool[1].runq));
	INIT_LIST_HEAD(&(thread_pool[1].freeq));
	thread_pool[1].pid = 1;

	int i = 2;
	for ( ; i < THREAD_NUM; ++i) {
		INIT_LIST_HEAD(&(thread_pool[i].runq));
		list_add_tail(&(thread_pool[i].freeq), &(free->freeq));
		thread_pool[i].pid = i;
	}
}

Thread*
create_kthread(void (*entry)(void)) {
	lock();
	Thread* new_thread = free;
	uint32_t* return_pointer = (uint32_t*)0;
	free = list_entry(free->freeq.next, Thread, freeq);
	list_add_tail(&(new_thread->runq), &(current->runq));
	list_del_init(&(new_thread->freeq));
	return_pointer = (uint32_t*)(&(new_thread->kstack[STK_SZ-1]) - sizeof(uint32_t) + 1);
	*return_pointer = (uint32_t)stop_thread;
	new_thread->tf = (struct TrapFrame*)((char*)(&(new_thread->kstack[STK_SZ-1])) - sizeof(struct TrapFrame) - sizeof(uint32_t) + 1);
	new_thread->tf->eax = new_thread->tf->ecx = 0;
	new_thread->tf->edx = new_thread->tf->ebx = 0;
	new_thread->tf->ebp = (uint32_t)(&(new_thread->kstack[STK_SZ]));
	new_thread->tf->edi = new_thread->tf->esi = 0;
	new_thread->tf->irq = 1000;
	new_thread->tf->eip = (uint32_t)entry;
	new_thread->tf->cs = 8;
	new_thread->tf->eflags = 0x200;
	new_thread->is_sleeping = 0;
	new_thread->lock_counter = 0;
	new_thread->unlock_status = 0;
	init_sem(&(new_thread->mutex), 1);
	init_sem(&(new_thread->amount), 0);
	register int i = 0;
	for (i = 0; i < MSGBUF_SZ; ++i) {
		INIT_LIST_HEAD(&(new_thread->msg_buf[i].msgq));
	}
	unlock();
	return new_thread;
}

void
sleep(void) {
	lock();
	next = list_entry(current->runq.next, Thread, runq);
	list_del_init(&(current->runq));
	if (sleeping == (Thread*)0) {
		sleeping = current;
		INIT_LIST_HEAD(&(current->runq));
	} else {
		list_add_tail(&(current->runq), &(sleeping->runq));
	}
	current->is_sleeping = 1;
	unlock();
	asm volatile ("int $0x80");
}

void
wakeup(Thread* t) {
	lock();
	if (t->is_sleeping) {
		if (sleeping == t) {
			if (list_empty(&(sleeping->runq))) {
				sleeping = (Thread*)0;
			} else {
				sleeping = list_entry(t->runq.next, Thread, freeq);
			}
		}
		list_del_init(&(t->runq));
		list_add_tail(&(t->runq), &(current->runq));
		t->is_sleeping = 0;
	}
	unlock();
}

void
stop_thread(void) {
	lock();
	next = list_entry(current->runq.next, Thread, runq);
	list_del_init(&(current->runq));
	list_add(&(current->freeq), &(free->freeq));
	asm volatile ("int $0x80");
}

