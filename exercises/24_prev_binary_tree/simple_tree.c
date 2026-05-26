#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!new_node) {
        return;
    }
    new_node->tree_node = tree_node;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        // 队列为空
        q->front = q->rear = new_node;
    } else {
        // 添加到队尾
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
    if (is_empty(q)) {
        return NULL;
    }
    
    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size <= 0 || level_order[0] == INT_MIN) {
        return NULL;
    }
    
    // 创建根节点
    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = NULL;
    root->right = NULL;
    
    Queue *q = create_queue();
    enqueue(q, root);
    
    int i = 1;
    while (!is_empty(q) && i < size) {
        TreeNode *current = dequeue(q);
        
        // 处理左子节点
        if (i < size) {
            if (level_order[i] != INT_MIN) {
                current->left = (TreeNode*)malloc(sizeof(TreeNode));
                current->left->val = level_order[i];
                current->left->left = NULL;
                current->left->right = NULL;
                enqueue(q, current->left);
            }
            i++;
        }
        
        // 处理右子节点
        if (i < size) {
            if (level_order[i] != INT_MIN) {
                current->right = (TreeNode*)malloc(sizeof(TreeNode));
                current->right->val = level_order[i];
                current->right->left = NULL;
                current->right->right = NULL;
                enqueue(q, current->right);
            }
            i++;
        }
    }
    
    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    
    // 使用栈来实现非递归前序遍历
    // 简单实现：使用数组作为栈
    int stack_size = 100;
    TreeNode **stack = (TreeNode**)malloc(sizeof(TreeNode*) * stack_size);
    int top = -1;
    
    // 根节点入栈
    stack[++top] = root;
    
    while (top >= 0) {
        // 弹出栈顶元素
        TreeNode *current = stack[top--];
        printf("%d ", current->val);
        
        // 先压入右子节点，再压入左子节点（这样左子节点先出栈）
        if (current->right != NULL) {
            stack[++top] = current->right;
        }
        if (current->left != NULL) {
            stack[++top] = current->left;
        }
    }
    
    free(stack);
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
