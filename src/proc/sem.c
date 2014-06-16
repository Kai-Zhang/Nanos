#include "common.h"
#include "x86/sem.h"
#include "x86/thread.h"

void
init_sem(Semaphore *sem, int value) {
	sem->count = value;
	INIT_LIST_HEAD(&sem->queue);
}

void
P(Semaphore *sem) {
	lock();
	sem->count --;
	if (sem->count < 0) {
		list_add(&sem->queue, &current->semq); // You should add ListHead semq in the Thread structure
		sleep(); // sleep!
	}
	unlock();
}

void
V(Semaphore *sem) {
	lock();
	sem->count ++;
	if (sem->count <= 0) {
		assert(!list_empty(&sem->queue));
		Thread *t = list_entry(sem->queue.next, Thread, semq);
		list_del(sem->queue.next);
		wakeup(t);
	}
	unlock();
}
