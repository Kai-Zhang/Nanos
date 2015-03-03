#ifndef __SEM_H__
#define __SEM_H__
#include "common/list.h"

struct Semaphore {
	int count;
	struct list_head queue;
};
typedef struct Semaphore Semaphore;

extern void init_sem(Semaphore *sem, int value);
extern void P(Semaphore *sem);
extern void V(Semaphore *sem);

#endif
