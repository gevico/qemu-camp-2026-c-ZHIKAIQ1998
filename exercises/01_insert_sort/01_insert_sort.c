#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    int score;
} Student;

void insertion_sort(Student students[], int n) {
    // 外层循环：从第二个元素开始，将每个元素插入到前面已排序的序列中
    for (int i = 1; i < n; i++) {
        Student key = students[i];  // 保存当前待插入的元素
        int j = i - 1;

        // 将大于 key.score 的元素向后移动一位
        while (j >= 0 && students[j].score < key.score) {
            students[j + 1] = students[j];
            j--;
        }
        // 将 key 放到正确的位置
        students[j + 1] = key;
    }
}

int main(void) {
    FILE *file;
    Student students[50];
    int n = 0;
    
    // 打开文件（从命令行参数获取文件名）
    file = fopen("01_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 01_students.txt\n");
        return 1;
    }
    
    // 从文件读取学生信息
    while (n < 50 && fscanf(file, "%s %d", students[n].name, &students[n].score) == 2) {
        n++;
    }
    fclose(file);
    
    if (n == 0) {
        printf("文件中没有学生信息\n");
        return 1;
    }
    
    insertion_sort(students, n);
    
    printf("\n按成绩从高到低排序后的学生信息:\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    
    return 0;
}

