#include "common.h"
#include "x86.h"
#include "device.h"
void grading (void);
void test(void);
void test_producer(void);

void thread_a (void) {
	while (1) {
		putchar('a');
	}
}
void thread_b (void) {
	while (1) {
		putchar('b');
	}
}
void thread_c (void) {
	while (1) {
		putchar('c');
	}
}
void
entry(void) {
//	init_timer();
	init_i8253();
	init_idt();
	init_intr();
	init_serial();
//	init_hal();
	init_thread();
//	init_tty();
	enable_interrupt();
	create_kthread(thread_a);
	create_kthread(thread_b);
	create_kthread(thread_c);
	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}

#define NBUF 5
#define NR_PROD 3
#define NR_CONS 4
 
int buf[NBUF], f = 0, r = 0, g = 1;
int last = 0;
Semaphore empty, full, mutex;
 
void
test_producer(void) {
	while (1) {
		P(&mutex);
		P(&empty);
		if(g % 10000 == 0) {
			printk(".");	// tell us threads are really working
		}
		buf[f ++] = g ++;
		f %= NBUF;
		V(&full);
		V(&mutex);
	}
}
 
void
test_consumer(void) {
	int get;
	while (1) {
		P(&mutex);
		P(&full);
		get = buf[r ++];
		assert(last == get - 1);	// the products should be strictly increasing
		last = get;
		r %= NBUF;
		V(&empty);
		V(&mutex);
	}
}
 
void
test_setup(void) {
	init_sem(&full, 0);
	init_sem(&empty, NBUF);
	init_sem(&mutex, 1);
	int i;
	for(i = 0; i < NR_PROD; i ++) {
		wakeup(create_kthread(test_producer));
	}
	for(i = 0; i < NR_CONS; i ++) {
		wakeup(create_kthread(test_consumer));
	}
}

void
grading (void) {
	
}

void
echo() {
	static int tty = 1;
	char name[] = "tty*", buf[256];
	Device *dev;
	lock();
	name[3] = '0' + (tty ++);
	unlock();
	while (1) {
		dev = hal_get(name);
		if (dev != NULL) {
			dev_write(dev, 0, name, 4);
			dev_write(dev, 0, "# ", 2);
			int i, nread = dev_read(dev, 0, buf, 255);
			buf[nread] = 0;
			for (i = 0; i < nread; i ++) {
				if (buf[i] >= 'a' && buf[i] <= 'z') {
					buf[i] += 'A' - 'a';
				}
			}
			dev_write(dev, 0, "Got: ", 5);
			dev_write(dev, 0, buf, nread);
			dev_write(dev, 0, "\n", 1);
		} else {
			printk("%s\n", name);
		}
	}
}

void
test() {
	int i;
	for (i = 0; i < NR_TTY; i ++) {
		wakeup(create_kthread(echo));
	}
}
