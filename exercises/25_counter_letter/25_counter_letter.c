#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// 定义BST节点结构
typedef struct TreeNode {
    char letter;        // 存储字母
    int count;          // 计数
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* create_node(char letter) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->letter = tolower(letter);  // 转换为小写
    newNode->count = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 向BST中插入节点或更新计数
TreeNode* insert_or_update(TreeNode* root, char letter) {
    if (root == NULL) {
        return create_node(letter);
    }
    
    char lower_letter = tolower(letter);
    
    if (lower_letter < root->letter) {
        root->left = insert_or_update(root->left, letter);
    } else if (lower_letter > root->letter) {
        root->right = insert_or_update(root->right, letter);
    } else {
        // 字母已存在，增加计数
        root->count++;
    }
    
    return root;
}

// 中序遍历BST并打印结果（按字母顺序）
void inorder_traversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    
    inorder_traversal(root->left);
    printf("%c:%d\n", root->letter, root->count);
    inorder_traversal(root->right);
}

// 释放BST内存
void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(int argc, char *argv[]) {
    const char* file_paths[] = {
        "paper.txt",
        "exercises/25_counter_letter/paper.txt",
        "../exercises/25_counter_letter/paper.txt"
    };
    int num_paths = sizeof(file_paths) / sizeof(file_paths[0]);
    
    FILE *file = NULL;
    
    for (int i = 0; i < num_paths; i++) {
        file = fopen(file_paths[i], "r");
        if (file != NULL) {
            break;
        }
    }
    
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    TreeNode* root = NULL;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {  // 只处理字母字符
            root = insert_or_update(root, c);
        }
    }
    
    fclose(file);
    
    // 按字母顺序输出结果
    inorder_traversal(root);
    
    // 释放内存
    free_tree(root);
    
    return 0;
}