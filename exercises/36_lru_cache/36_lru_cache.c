#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    /* 简单哈希函数: 取绝对值后对桶数取模 */
    return (unsigned)((key < 0 ? -key : key) % 16);
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned idx = hash_int(key);
    HashEntry** prev_next = &c->buckets[idx];
    
    while (*prev_next != NULL) {
        if ((*prev_next)->key == key) {
            if (pprev_next) {
                *pprev_next = prev_next;
            }
            return *prev_next;
        }
        prev_next = &((*prev_next)->next);
    }
    
    if (pprev_next) {
        *pprev_next = prev_next;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    
    if (c->head != NULL) {
        c->head->prev = node;
    } else {
        /* 链表为空,tail 也指向新节点 */
        c->tail = node;
    }
    
    c->head = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        /* node 是 head */
        c->head = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        /* node 是 tail */
        c->tail = node->prev;
    }
    
    node->prev = NULL;
    node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    if (c->head == node) {
        return; /* 已经在头部,无需移动 */
    }
    
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    LRUNode* tail = c->tail;
    if (tail != NULL) {
        list_remove(c, tail);
    }
    return tail;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }
    
    LRUCache* c = malloc(sizeof(LRUCache));
    if (!c) {
        return NULL;
    }
    
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    c->bucket_count = 16;
    
    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    
    return c;
}

static void lru_free(LRUCache* c) {
    if (!c) {
        return;
    }
    
    /* 释放所有哈希表项和节点 */
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* entry = c->buckets[i];
        while (entry != NULL) {
            HashEntry* next = entry->next;
            free(entry->node);  /* 释放链表节点 */
            free(entry);        /* 释放哈希表项 */
            entry = next;
        }
    }
    
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (!entry) {
        return 0; /* 未找到 */
    }
    
    /* 移动到头部 (标记为最近使用) */
    list_move_to_head(c, entry->node);
    
    if (out_value) {
        *out_value = entry->node->value;
    }
    
    return 1; /* 找到 */
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry* entry = NULL;
    HashEntry** prev_next = NULL;
    
    /* 检查 key 是否已存在 */
    entry = hash_find(c, key, &prev_next);
    
    if (entry != NULL) {
        /* key 已存在,更新值并移动到头部 */
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    
    /* key 不存在,创建新节点 */
    if (c->size >= c->capacity) {
        /* 容量已满,淘汰 LRU 节点 */
        LRUNode* lru_node = list_pop_tail(c);
        if (lru_node) {
            /* 从哈希表中删除对应项 */
            unsigned idx = hash_int(lru_node->key);
            HashEntry** prev = &c->buckets[idx];
            while (*prev != NULL) {
                if ((*prev)->node == lru_node) {
                    HashEntry* to_delete = *prev;
                    *prev = to_delete->next;
                    free(to_delete);
                    break;
                }
                prev = &((*prev)->next);
            }
            free(lru_node);
            c->size--;
        }
    }
    
    /* 创建新节点并添加到头部 */
    LRUNode* new_node = malloc(sizeof(LRUNode));
    if (!new_node) {
        return;
    }
    
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    
    list_add_to_head(c, new_node);
    
    /* 添加到哈希表 */
    unsigned idx = hash_int(key);
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    if (!new_entry) {
        free(new_node);
        return;
    }
    
    new_entry->key = key;
    new_entry->node = new_node;
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;
    
    c->size++;
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
