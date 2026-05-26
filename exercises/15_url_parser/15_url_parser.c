#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // 找到 '?' 的位置，参数从 '?' 之后开始
    const char* query_start = strchr(url, '?');
    if (query_start == NULL) {
        printf("未找到查询参数\n");
        goto exit;
    }
    
    // 跳过 '?' 字符
    query_start++;
    
    // 复制参数字符串，因为strtok会修改原字符串
    char* query_str = strdup(query_start);
    if (query_str == NULL) {
        printf("内存分配失败\n");
        err = -1;
        goto exit;
    }
    
    // 使用 '&' 分割各个参数
    char* saveptr = NULL;
    char* param = strtok_r(query_str, "&", &saveptr);
    
    while (param != NULL) {
        // 在每个参数中查找 '=' 来分割key和value
        char* equal_sign = strchr(param, '=');
        if (equal_sign != NULL) {
            // 将 '=' 替换为 '\0' 来分割字符串
            *equal_sign = '\0';
            char* key = param;
            char* value = equal_sign + 1;
            
            // 输出键值对
            printf("key = %s, value = %s\n", key, value);
        }
        
        // 获取下一个参数
        param = strtok_r(NULL, "&", &saveptr);
    }
    
    free(query_str);

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}