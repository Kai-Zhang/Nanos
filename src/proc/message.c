#include "x86/message.h"
#include "x86/thread.h"
#include "common.h"

int
send(pid_t dst, Message *msg) {
	if (is_irq) {
		msg->src = MSG_HWINTR;
	} else {
		msg->src = current->pid;
	}
	msg->dest = dst;
	P(&(thread_pool[dst].mutex));
	Message *new_msg = NULL;
	register int i = 0;
	for (i = 0; i < MSGBUF_SZ; ++i) {
		if (thread_pool[dst].messages != &thread_pool[dst].msg_buf[i] &&
			list_empty(&(thread_pool[dst].msg_buf[i].msgq))) {
			new_msg = &thread_pool[dst].msg_buf[i];
			break;
		}
	}
	assert(new_msg);
	memcpy(new_msg, msg, sizeof(Message));
	if (!thread_pool[dst].messages) {
		thread_pool[dst].messages = new_msg;
	} else {
		list_add_tail(&(new_msg->msgq), &(thread_pool[dst].messages->msgq));
	}
	V(&(thread_pool[dst].mutex));
	V(&(thread_pool[dst].amount));
	return 1;
}

int
receive(pid_t src, Message *msg) {
	Message *trv = current->messages;
	msg = NULL;
	while (1) {
		P(&(current->amount));
		P(&(current->mutex));
		if (src == ANY || current->messages->src == src) {
			msg = current->messages;
			break;
		}
		list_for_each_entry(trv, &(current->messages->msgq), msgq) {
			if (trv->src == src) {
				msg = trv;
				break;
			}
		}
		if (!msg) {
			V(&(current->mutex));
			V(&(current->amount));
			continue;
		} else {
			break;
		}
	}
	list_del_init(&(msg->msgq));
	V(&(current->mutex));
	return 1;
}

