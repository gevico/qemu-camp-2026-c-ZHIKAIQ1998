#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG_LEVEL

#if DEBUG_LEVEL >= 1
#define DEBUG_PRINT(fmt, ...) \
    do { \
        printf("DEBUG: func=%s, line=%d", __func__, __LINE__); \
        if (DEBUG_LEVEL >= 2) { \
            printf(", " fmt, ##__VA_ARGS__); \
        } \
        printf("\n"); \
        if (DEBUG_LEVEL >= 3) { \
            void *buffer[100]; \
            int nptrs = backtrace(buffer, 100); \
            char **strings = backtrace_symbols(buffer, nptrs); \
            if (strings != NULL) { \
                printf("Backtrace:\n"); \
                for (int i = 0; i < nptrs; i++) { \
                    printf("  %s\n", strings[i]); \
                } \
                free(strings); \
            } \
        } \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif







// 测试代码

void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}