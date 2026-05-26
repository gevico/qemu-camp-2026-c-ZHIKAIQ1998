#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // 跳过 "s/"
    const char* start = cmd + 2;
    
    // 找到第一个 '/' 的位置，这是 old_str 的结束
    const char* first_slash = strchr(start, '/');
    if (first_slash == NULL) {
        return -1;
    }
    
    // 找到第二个 '/' 的位置，这是 new_str 的结束
    const char* second_slash = strchr(first_slash + 1, '/');
    if (second_slash == NULL) {
        return -1;
    }
    
    // 计算 old_str 的长度并复制
    size_t old_len = first_slash - start;
    *old_str = (char*)malloc(old_len + 1);
    if (*old_str == NULL) {
        return -1;
    }
    strncpy(*old_str, start, old_len);
    (*old_str)[old_len] = '\0';
    
    // 计算 new_str 的长度并复制
    size_t new_len = second_slash - (first_slash + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (*new_str == NULL) {
        free(*old_str);
        return -1;
    }
    strncpy(*new_str, first_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 查找 old 在 str 中第一次出现的位置
    char* pos = strstr(str, old);
    if (pos == NULL) {
        // 如果没有找到，直接返回
        return;
    }
    
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t str_len = strlen(str);
    
    // 如果新旧字符串长度相同，直接替换
    if (old_len == new_len) {
        memcpy(pos, new, new_len);
    } 
    // 如果新字符串更短
    else if (new_len < old_len) {
        // 先复制新字符串
        memcpy(pos, new, new_len);
        // 将后面的内容向前移动
        memmove(pos + new_len, pos + old_len, str_len - old_len - (pos - str) + 1);
    } 
    // 如果新字符串更长
    else {
        // 计算需要移动的字节数
        size_t tail_len = str_len - (pos - str) - old_len;
        // 先将后面的内容向后移动
        memmove(pos + new_len, pos + old_len, tail_len + 1);
        // 再复制新字符串
        memcpy(pos, new, new_len);
    }
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
