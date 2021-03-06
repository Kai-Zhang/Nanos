#include "device/hal.h"
#include "device/serial.h"
#include "x86/thread.h"
#include "x86/message.h"
#include "common.h"
#define NR_DEV 64

static Device dev_pool[NR_DEV];
static list_head freeq, devices;

void
init_hal(void) {
	int i = 0;
	INIT_LIST_HEAD(&freeq);
	for (i = 0; i < NR_DEV; i ++) {
		list_add(&dev_pool[i].list, &freeq);
	}
	INIT_LIST_HEAD(&devices);
}

void
hal_register(const char *name, pid_t pid, int dev_id) {
	lock();
	if (list_empty(&freeq)) {
		panic("no room for more device");
	}
	Device *dev = list_entry(freeq.next, Device, list);
	list_del(&dev->list);
	dev->name = name;
	dev->pid = pid;
	dev->dev_id = dev_id;
	list_add(&dev->list, &devices);
	unlock();
}

void
hal_list(void) {
	lock();
	list_head *it;
	printk("listing all registed devices:\n");
	list_for_each(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		printk("%s #%d, #%d\n", dev->name, dev->pid, dev->dev_id);
	}
	unlock();
}

Device*
hal_get(const char *name) {
	lock();
	list_head *it;
	list_for_each(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		if (strcmp(dev->name, name) == 0) {
			unlock();
			return dev;
		}
	}
	unlock();
	return NULL;
}

static size_t
dev_rw(int type, Device *dev, off_t offset, void *buf, size_t count) {
	DevMessage m;
//	assert(sizeof(DevMessage) <= sizeof(Message)); // Message结构体不能定义得太小
	m.header.type = type;
	m.dev_id = dev->dev_id;
	m.offset = offset;
	m.buf = buf;
	m.count = count;
	send(dev->pid, (Message*)&m);
	receive(dev->pid, (Message*)&m);
	return m.header.type;
}

size_t
dev_read(Device *dev, off_t offset, void *buf, size_t count) {
	return dev_rw(MSG_DEVRD, dev, offset, buf, count);
}

size_t
dev_write(Device *dev, off_t offset, void *buf, size_t count) {
	return dev_rw(MSG_DEVWR, dev, offset, buf, count);
}

