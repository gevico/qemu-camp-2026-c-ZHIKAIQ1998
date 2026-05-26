#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    // 解析格式: s/old/new/
    if (strlen(cmd) < 4 || cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }
    
    // 找到第二个 '/' 的位置
    const char* second_slash = strchr(cmd + 2, '/');
    if (!second_slash) {
        return -1;
    }
    
    // 提取 old_str
    size_t old_len = second_slash - (cmd + 2);
    *old_str = malloc(old_len + 1);
    if (!*old_str) {
        return -1;
    }
    strncpy(*old_str, cmd + 2, old_len);
    (*old_str)[old_len] = '\0';
    
    // 找到第三个 '/' 的位置
    const char* third_slash = strchr(second_slash + 1, '/');
    if (!third_slash) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    
    // 提取 new_str
    size_t new_len = third_slash - (second_slash + 1);
    *new_str = malloc(new_len + 1);
    if (!*new_str) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    strncpy(*new_str, second_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // 查找 old 在 str 中的位置
    char* pos = strstr(str, old);
    if (!pos) {
        return;  // 未找到,不替换
    }
    
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t prefix_len = pos - str;
    size_t suffix_len = strlen(pos + old_len);
    
    // 计算新字符串长度
    size_t new_str_len = prefix_len + new_len + suffix_len;
    
    // 创建临时缓冲区
    char temp[1024];
    if (new_str_len >= sizeof(temp)) {
        return;  // 防止溢出
    }
    
    // 构建新字符串: 前缀 + new + 后缀
    strncpy(temp, str, prefix_len);
    temp[prefix_len] = '\0';
    strcat(temp, new);
    strcat(temp, pos + old_len);
    
    // 复制回原字符串
    strcpy(str, temp);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}