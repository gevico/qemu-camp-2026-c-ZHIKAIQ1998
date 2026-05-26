#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

int main() {
    Queue q;
    int total_people = 50;
    int report_interval = 5;

    // 初始化队列
    q.head = 0;
    q.tail = 0;
    q.count = 0;

    // 所有人入队（编号 1 ~ 50）
    for (int i = 1; i <= total_people; i++) {
        q.data[q.tail].id = i;
        q.tail = (q.tail + 1) % MAX_PEOPLE;
        q.count++;
    }

    // 模拟约瑟夫环淘汰过程，直到只剩一人
    int round = 1;
    while (q.count > 1) {
        // 前 report_interval-1 个人出队后重新入队（报数未到5）
        for (int i = 0; i < report_interval - 1; i++) {
            People person = q.data[q.head];
            q.head = (q.head + 1) % MAX_PEOPLE;
            q.count--;

            q.data[q.tail] = person;
            q.tail = (q.tail + 1) % MAX_PEOPLE;
            q.count++;
        }
        // 第 report_interval 个人出队（淘汰）
        People eliminated = q.data[q.head];
        q.head = (q.head + 1) % MAX_PEOPLE;
        q.count--;

        printf("淘汰: %d\n",eliminated.id);
        round++;
    }

    // 输出最后剩下的人
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}