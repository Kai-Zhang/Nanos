#include "x86/message.h"

int
send(pid_t dst, Message *msg) {
	return 1;
}

int
receive(pid_t dst, Message *msg) {
	return 1;
}

int
add_irq_handle(int itr_num, void(*handle)(void)) {
	return 1;
}

