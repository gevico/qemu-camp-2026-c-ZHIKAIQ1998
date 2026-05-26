// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_lowercase(char *str);  // 前置声明

void trim(char *str) {
    // 去除字符串首尾的空格和制表符
    if (!str) return;
    
    // 跳过前导空格
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    
    // 如果全是空格
    if (*start == '\0') {
        *str = '\0';
        return;
    }
    
    // 找到末尾位置
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // 截断字符串
    *(end + 1) = '\0';
    
    // 移动字符串到开头
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};

  while (fgets(line, sizeof(line), file) != NULL) {
    // 去除换行符
    line[strcspn(line, "\n")] = '\0';
    
    // 跳过空行
    if (strlen(line) == 0) {
      continue;
    }
    
    // 检查是否是单词行(以 # 开头)
    if (line[0] == '#') {
      // 如果之前有未完成的词条,先保存
      if (strlen(current_word) > 0 && strlen(current_translation) > 0) {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
      }
      
      // 提取单词(跳过 # 号)
      const char* word_start = line + 1;
      // 跳过前导空格
      while (*word_start == ' ' || *word_start == '\t') {
        word_start++;
      }
      
      strncpy(current_word, word_start, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      to_lowercase(current_word);
      current_translation[0] = '\0';
    } else if (strncmp(line, "Trans:", 6) == 0) {
      // 翻译行(以 Trans: 开头)
      const char* trans_start = line + 6;
      // 跳过前导空格
      while (*trans_start == ' ' || *trans_start == '\t') {
        trans_start++;
      }
      
      if (strlen(current_translation) > 0) {
        strcat(current_translation, "@");
      }
      strcat(current_translation, trans_start);
    }
  }
  
  // 处理最后一个词条
  if (strlen(current_word) > 0 && strlen(current_translation) > 0) {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  
  // 尝试多个可能的路径
  const char* dict_paths[] = {
    "src/mytrans/dict.txt",
    "../src/mytrans/dict.txt",
    "../../src/mytrans/dict.txt",
    "/workspace/exercises/20_mybash/src/mytrans/dict.txt",
    NULL
  };
  
  int loaded = 0;
  for (int i = 0; dict_paths[i] != NULL; i++) {
    FILE* test_file = fopen(dict_paths[i], "r");
    if (test_file) {
      fclose(test_file);
      if (load_dictionary(dict_paths[i], table, &dict_count) == 0) {
        loaded = 1;
        break;
      }
    }
  }
  
  if (!loaded) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      const char *translation = hash_table_lookup(table, word);
      printf("原文: %s\t", word);
      if (translation) {
          printf("翻译: %s\n", translation);
      } else {
          printf("未找到该单词的翻译。\n");
      }

      word = strtok(NULL, " ");
    }
  }

  free_hash_table(table);
  return 0;
}