#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
Student temp[MAX_STUDENTS];

void merge_sort(int left, int right) {
    // 递归终止条件：区间长度为 1 或 0
    if (left >= right) {
        return;
    }

    // 1. 划分：计算中间点，避免整数溢出
    int mid = left + (right - left) / 2;

    // 2. 递归：分别对左右两半排序
    merge_sort(left, mid);
    merge_sort(mid + 1, right);

    // 3. 归并：将两个有序子序列合并到 temp，再拷贝回 students
    int i = left;       // 左半部分指针
    int j = mid + 1;    // 右半部分指针
    int k = left;       // 临时数组指针

    // 合并两个有序子数组（降序）
    while (i <= mid && j <= right) {
        if (students[i].score >= students[j].score) {
            temp[k++] = students[i++];
        } else {
            temp[k++] = students[j++];
        }
    }

    // 将左半部分剩余元素复制到 temp
    while (i <= mid) {
        temp[k++] = students[i++];
    }

    // 将右半部分剩余元素复制到 temp
    while (j <= right) {
        temp[k++] = students[j++];
    }

    // 将归并结果从 temp 拷贝回 students 对应区间
    for (int idx = left; idx <= right; idx++) {
        students[idx] = temp[idx];
    }
}

int main(void) {
    FILE *file = fopen("02_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 02_students.txt\n");
        return 1;
    }

    int n;
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

    merge_sort(0, n - 1);

    printf("\n归并排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}