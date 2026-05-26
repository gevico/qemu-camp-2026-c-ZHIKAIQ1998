#include <stdio.h>
#include "singly_linked_list.h"

void print_item(link p) { printf("%d ", p->item); }

int main() {
    // 创建链表
    for (int i = 5; i >= 1; i--) {
        link new_node = make_node(i);
        push(new_node);
    }
    
    printf("链表内容: ");
    traverse(print_item);
    printf("\n");
    
    // 测试删除
    link head_node = NULL;
    void (*capture)(link) = [](link p) { if (!head_node) head_node = p; };
    
    return 0;
}
