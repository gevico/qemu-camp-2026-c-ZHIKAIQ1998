#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 50
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
int n;
int linear_search(const char *target_name) {
    // 遍历学生数组，逐个比较姓名
    for (int i = 0; i < n; i++) {
        // 使用 strcmp 比较字符串，返回 0 表示匹配
        if (strcmp(students[i].name, target_name) == 0) {
            return i;   // 找到目标，返回索引
        }
    }
    return -1;          // 遍历结束未找到，返回 -1
}
int main(void) {
    FILE *file = fopen("04_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 04_students.txt\n");
        return 1;
    }

    fscanf(file, "%d", &n);
    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d\n", n);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    char query_name[NAME_LEN] = "David";

    int index = linear_search(query_name);

    printf("\n线性查找出的学生信息：\n");
    if (index != -1) {
        printf("姓名：%s，成绩：%d\n", students[index].name, students[index].score);
    } else {
        printf("未找到该学生\n");
    }

    return 0;
}