// main.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int main() {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }

  FILE* file = fopen("text.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 text.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok_r 按空格分割单词
    char *saveptr;
    char *word = strtok_r(line, " \t", &saveptr);
    
    while (word != NULL) {
      // 转换为小写
      to_lowercase(word);
      
      // 去除单词末尾的标点符号(保留字母、数字和连字符)
      int len = strlen(word);
      while (len > 0 && !isalpha((unsigned char)word[len-1]) && !isdigit((unsigned char)word[len-1])) {
        word[len-1] = '\0';
        len--;
      }
      
      // 跳过空字符串
      if (strlen(word) == 0) {
        word = strtok_r(NULL, " \t", &saveptr);
        continue;
      }
      
      // 查找翻译
      const char *translation = hash_table_lookup(table, word);
      
      if (translation) {
        printf("原文: %s\t翻译: %s\n", word, translation);
      } else {
        printf("原文: %s\t未找到该单词的翻译。\n", word);
      }
      
      word = strtok_r(NULL, " \t", &saveptr);
    }
  }

  free_hash_table(table);
  return 0;
}
