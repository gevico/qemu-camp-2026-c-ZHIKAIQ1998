#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data) {
    link p = (link)malloc(sizeof(struct node));
    if (p) {
        p->data = data;
        p->prev = NULL;
        p->next = NULL;
    }
    return p;
}

void free_node(link p) {
    if (p) {
        free(p);
    }
}

link search(int key) {
    link current = head->next;
    while (current != tail) {
        if (current->data == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void insert(link p) {
    // 头插法：插入到 head 之后
    p->next = head->next;
    p->prev = head;
    head->next->prev = p;
    head->next = p;
}

void delete(link p) {
    // 从链表中移除节点（不释放内存）
    p->prev->next = p->next;
    p->next->prev = p->prev;
    p->prev = NULL;
    p->next = NULL;
}

void traverse(void (*visit)(link)) {
    link current = head->next;
    while (current != tail) {
        visit(current);
        current = current->next;
    }
}

void destroy(void) {
    link current = head->next;
    while (current != tail) {
        link next = current->next;
        free(current);
        current = next;
    }
    // 重置哨兵节点的连接
    head->next = tail;
    tail->prev = head;
}
