#ifndef __X86_MESSAGE_H__
#define __X86_MESSAGE_H__
#include "common/types.h"
#include "common/list.h"

#define ANY -1
#define MSG_SZ 2048

struct MsgHead {
	int type;
	pid_t src, dest;
};
typedef struct MsgHead MsgHead;

struct Message {
	int type;
	pid_t src, dest;
	char payload[MSG_SZ];
	list_head msgq;
};
typedef struct Message Message;

#define MSG_HWINTR 0
int send(pid_t, Message*);
int receive(pid_t, Message*);

#endif
