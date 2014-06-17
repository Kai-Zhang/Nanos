#ifndef __X86_MESSAGE_H__
#define __X86_MESSAGE_H__
#include "common/types.h"

struct MsgHead {
	int type;
	int src;
};
typedef struct MsgHead MsgHead;

struct Message {
	int type;
	int src;
};
typedef struct Message Message;

#define ANY -1
#define MSG_HWINTR 0
int send(pid_t, Message*);
int receive(pid_t, Message*);
int add_irq_handle(int, void(*)(void));

#endif
