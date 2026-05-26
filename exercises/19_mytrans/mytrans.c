#define _GNU_SOURCE
// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    if (!str) return;

    // 去除前导空格
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // 如果全是空格
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // 移动字符串到开头
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    // 去除尾部空格
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
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
  int in_entry = 0;

  *dict_count = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    // 去除换行符
    line[strcspn(line, "\n")] = '\0';

    // 跳过空行
    if (strlen(line) == 0) continue;

    // 检查是否是单词行(以 # 开头)
    if (line[0] == '#') {
      // 如果之前有未处理的词条,先保存
      if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
      }

      // 提取新单词(去掉 # 前缀)
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      trim(current_word);

      // 清空翻译
      current_translation[0] = '\0';
      in_entry = 1;
    }
    // 检查是否是翻译行(以 Trans: 开头)
    else if (strncmp(line, "Trans:", 6) == 0) {
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1] = '\0';
      trim(current_translation);
    }
  }

  // 处理最后一个词条
  if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}
