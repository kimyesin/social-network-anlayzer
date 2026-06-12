#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// 31 곱하는 이유: 소수라서 글자 순서 달라도 해시값이 달라짐
static int hash_func(char *name) {
    int result = 0;
    while (*name) {
        result = result * 31 + *name;
        name++;
    }
    // 음수 방지
    return (result % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
}

HashTable *hash_create() {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht == NULL) return NULL;
    for (int i = 0; i < TABLE_SIZE; i++)
        ht->table[i] = NULL;
    return ht;
}

void hash_insert(HashTable *ht, char *name, int id) {
    int idx = hash_func(name);
    HashEntry *item = (HashEntry *)malloc(sizeof(HashEntry));
    if (item == NULL) return;
    strncpy(item->name, name, MAX_USERNAME - 1);
    item->name[MAX_USERNAME - 1] = '\0';
    item->user_id = id;
    item->next = ht->table[idx];
    ht->table[idx] = item;
}

int hash_search(HashTable *ht, char *name) {
    int idx = hash_func(name);
    HashEntry *cur = ht->table[idx];
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0)
            return cur->user_id;
        cur = cur->next;
    }
    return -1;
}

void hash_delete(HashTable *ht, char *name) {
    int idx = hash_func(name);
    HashEntry *cur = ht->table[idx];
    HashEntry *prev = NULL;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            if (prev == NULL)
                ht->table[idx] = cur->next;
            else
                prev->next = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void hash_free(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry *cur = ht->table[i];
        while (cur != NULL) {
            HashEntry *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(ht);
}