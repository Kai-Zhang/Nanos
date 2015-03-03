#include "common/string.h"

void *
memcpy(void *dst, const void *src, size_t count) {
    void *ret = dst;
    while (count --) {
        *(char *)dst = *(char *)src;
        dst = (char *)dst + 1;
        src = (char *)src + 1;
    }
    return ret;
}

void *
memset(void *dst, int val, size_t count) {
    void *start = dst;
    while (count --) {
        *(char *)dst = (char)val;
        dst = (char *)dst + 1;
    }
    return start;
}

int
strcmp(const void *str1, const void *str2) {
	const uint8_t *trv1 = str1, *trv2 = str2;
	while (*trv1 ++ == *trv2 ++) {
		if (*trv1 == 0) {
			return 0;
		}
	}
	return *trv1 - *trv2;
}

