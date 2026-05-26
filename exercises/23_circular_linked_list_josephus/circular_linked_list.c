#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    // 创建第一个节点
    Node* head = (Node*)malloc(sizeof(Node));
    if (!head) {
        return NULL;
    }
    head->id = 1;
    head->next = NULL;
    
    // 创建剩余节点并链接
    Node* current = head;
    for (int i = 2; i <= n; i++) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        if (!new_node) {
            // 内存分配失败，释放已创建的节点
            free_list(head);
            return NULL;
        }
        new_node->id = i;
        new_node->next = NULL;
        
        current->next = new_node;
        current = new_node;
    }
    
    // 尾节点指向头节点，形成环形
    current->next = head;
    
    return head;
}

void free_list(Node* head) {
    if (!head) {
        return;
    }
    
    // 断开环形，避免无限循环
    Node* current = head->next;
    head->next = NULL;
    
    // 释放所有节点
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    
    // 释放头节点
    free(head);
}
