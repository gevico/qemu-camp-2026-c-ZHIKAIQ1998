#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    while (*delim) {
        if (c == *delim) {
            return 1;
        }
        delim++;
    }
    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *start;

    /* 第一次调用: str 非NULL,从 str 开始 */
    /* 后续调用: str 为NULL,从 saveptr 继续 */
    if (str != NULL) {
        start = str;
    } else {
        start = *saveptr;
    }

    /* 如果已经没有字符串可处理了 */
    if (start == NULL || *start == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    /* 跳过开头的分隔符 */
    while (*start && is_delim(*start, delim)) {
        start++;
    }

    /* 如果跳过分隔符后到达字符串末尾 */
    if (*start == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    /* 找到token的起始位置 */
    char *token = start;

    /* 找到token的结束位置(下一个分隔符或字符串末尾) */
    while (*start && !is_delim(*start, delim)) {
        start++;
    }

    /* 如果找到分隔符,将其替换为'\0'并更新saveptr */
    if (*start) {
        *start = '\0';
        *saveptr = start + 1;
    } else {
        /* 到达字符串末尾 */
        *saveptr = NULL;
    }

    return token;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
