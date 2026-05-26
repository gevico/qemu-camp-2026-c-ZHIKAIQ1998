#include <stdio.h>

// 自定义字符串拷贝函数
void my_strcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // 别忘了拷贝结束符
}

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
    // 调用自定义拷贝函数
    my_strcpy(destination, source);
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}