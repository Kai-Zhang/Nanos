#include "device/tty.h"
#include "x86/message.h"
#include "x86/irq.h"

pid_t TTY;

void ttyd(void);
void send_keymsg(void);

void init_console();

void init_tty(void) {
	add_irq_handle(1, send_keymsg); // 在irq1时，调用send_keymsg函数
	init_console();
}

