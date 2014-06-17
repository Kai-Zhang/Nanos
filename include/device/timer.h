#ifndef __TIMER_H__
#define __TIMER_H__

#define PORT_TIME 0x40
#define PORT_RTC  0x70
/* 8253输入频率为1.193182MHz */
#define FREQ_8253 1193182

#define HZ 100

struct Time {
	int year, month, day;
	int hour, minute, second;
};
typedef struct Time Time;

extern long jiffy;

void init_timer(void);
void timerd(void);
void init_i8253(void);
void update_sched(void);
void update_jiffy(void);
int read_rtc(int);
void get_time(Time *tm);

#endif
