#include <stdio.h>
#include <ctype.h>   // 提供 isalpha 函数

int main() {
    char str[] = "Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    
    int in_word = 0;  // 标志：当前是否处于单词内部

    for (int i = 0; str[i] != '\0'; i++) {
        // 字母或撇号视为单词的一部分
        if (isalpha(str[i]) || str[i] == '\'') {
            if (!in_word) {          // 新单词开始
                wordCount++;
                in_word = 1;
            }
        } else {
            in_word = 0;             // 遇到其他字符（空格、逗号、句点等），单词结束
        }
    }
    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}