#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    link new_node = (link)malloc(sizeof(struct node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->item = item;
    new_node->next = NULL;
    return new_node;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    link current = head;
    while (current != NULL) {
        if (current->item == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 在链表头部插入节点
void insert(link p) {
    if (p == NULL) return;
    p->next = head;
    head = p;
}

// 删除指定节点
void delete(link p) {
    if (p == NULL || head == NULL) return;
    
    // 如果要删除的是头节点
    if (head == p) {
        head = head->next;
        free_node(p);
        return;
    }
    
    // 查找要删除节点的前一个节点
    link current = head;
    while (current->next != NULL && current->next != p) {
        current = current->next;
    }
    
    // 如果找到了前一个节点
    if (current->next == p) {
        current->next = p->next;
        free_node(p);
    }
}

// 遍历链表
void traverse(void (*visit)(link)) {
    link current = head;
    while (current != NULL) {
        visit(current);
        current = current->next;
    }
}

// 销毁整个链表
void destroy(void) {
    link current = head;
    while (current != NULL) {
        link next = current->next;
        free_node(current);
        current = next;
    }
    head = NULL;
}

// 在链表头部推入节点
void push(link p) { 
    insert(p);
 }

// 从链表头部弹出节点
link pop(void) {
    if (head == NULL) {
        return NULL;
    }
    link top = head;
    head = head->next;
    top->next = NULL;
    return top;
}

// 释放链表内存
void free_list(link list_head) {
    link current = list_head;
    while (current != NULL) {
        link next = current->next;
        free_node(current);
        current = next;
    }
}
