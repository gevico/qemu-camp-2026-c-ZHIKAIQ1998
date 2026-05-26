#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];

void quick_sort(int left, int right) {
    // 递归终止条件：区间长度 <= 1 时已经有序
    if (left >= right) {
        return;
    }

    // 选择最右边的元素作为基准（pivot）
    int pivot = students[right].score;
    // i 指向小于基准（在降序中为大于）的区域的最后一个位置
    int i = left - 1;

    // 分区操作：将大于等于 pivot 的元素移到左边，小于 pivot 的移到右边
    for (int j = left; j < right; j++) {
        if (students[j].score >= pivot) {  // 降序：分数高的放左边
            i++;
            // 交换 students[i] 和 students[j]
            Student temp = students[i];
            students[i] = students[j];
            students[j] = temp;
        }
    }
    // 将基准元素放到正确的位置（i+1）
    Student temp = students[i + 1];
    students[i + 1] = students[right];
    students[right] = temp;

    int pivot_index = i + 1;

    // 递归排序左右两部分
    quick_sort(left, pivot_index - 1);
    quick_sort(pivot_index + 1, right);
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}