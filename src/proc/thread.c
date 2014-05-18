#include "x86.h"
#include "common.h"

Thread thread_stack[THREAD_NUM];
Thread *current, *running, *next, *sleeping;
volatile int lock_counter;

void
init_thread(void) {
	int i = 0;
	for ( ; i < THREAD_NUM; ++i) {
		INIT_LIST_HEAD(&(thread_stack[i].runq));
		INIT_LIST_HEAD(&(thread_stack[i].freeq));
	}
	current = &thread_stack[0];
	running = current;
	next = &thread_stack[1];
//	asm volatile ("movl %0, %%esp" :
//			: "a" (&(thread_stack[0].kstack[STK_SZ]))
//			: "%esp");
//	asm volatile ("movl %esp, %ebp");
}

Thread*
create_kthread(void (*entry)(void)) {
	lock();
	Thread* new_thread = next;
	list_add_tail(&(new_thread->runq), &(current->runq));
	INIT_LIST_HEAD(&(new_thread->freeq));
	for (next = &thread_stack[1];
		!list_empty(&(next->runq)) || !list_empty(&(next->freeq));
		++ next) ;
	new_thread->tf = (struct TrapFrame*)((char*)(&(new_thread->kstack[STK_SZ-1])) - sizeof(struct TrapFrame));
	new_thread->tf->lck = 0;
	new_thread->tf->eax = new_thread->tf->ecx = 0;
	new_thread->tf->edx = new_thread->tf->ebx = 0;
	new_thread->tf->ebp = (uint32_t)(&(new_thread->kstack[STK_SZ]));
	new_thread->tf->edi = new_thread->tf->esi = 0;
	new_thread->tf->irq = 1000;
	new_thread->tf->eip = (uint32_t)entry;
	new_thread->tf->cs = 8;
	new_thread->tf->eflags = 0x200;
	new_thread->is_sleeping = 0;
	unlock();
	return new_thread;
}

void
sleep(void) {
	lock();
	if (sleeping == (Thread*)0) {
		sleeping = current;
		INIT_LIST_HEAD(&(current->freeq));
	} else {
		list_add_tail(&(current->freeq), &(sleeping->freeq));
	}
	list_del_init(&(current->runq));
	current->is_sleeping = 1;
	running = list_entry(current->runq.next, Thread, runq);
	unlock();
	asm volatile ("int $0x80");
}

void
wakeup(Thread* t) {
	lock();
	if (t->is_sleeping) {
		if (sleeping == t) {
			if (list_empty(&(sleeping->freeq))) {
				sleeping = (Thread*)0;
			} else {
				sleeping = list_entry(t->freeq.next, Thread, freeq);
			}
		}
		list_del_init(&(t->freeq));
		list_add_tail(&(t->runq), &(current->runq));
		t->is_sleeping = 0;
	}
	unlock();
}

void
lock(void) {
	disable_interrupt();
	++ lock_counter;
}

void
unlock(void) {
	-- lock_counter;
	if (lock_counter <= 0) {
		enable_interrupt();
	}
}

