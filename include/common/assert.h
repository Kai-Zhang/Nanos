#ifndef __ASSERT_H__
#define __ASSERT_H__

int abort(const char *, int);
int panic(const char *);

/* panic: 退出系统 */
#define panic(...) \
	abort(__FILE__,__LINE__)

/* assert: 断言条件为真，若为假则蓝屏退出 */
#define assert(cond) \
	((cond) ? (0) : (abort(__FILE__, __LINE__)))

#endif

