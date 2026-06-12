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
    return (result % MAP_SIZE + MAP_SIZE) % MAP_SIZE;
}

HashMap *hmap_create() {
    HashMap *hm = (HashMap *)malloc(sizeof(HashMap));
    if (hm == NULL) return NULL;
    for (int i = 0; i < MAP_SIZE; i++)
        hm->slots[i] = NULL;
    return hm;
}

void hmap_put(HashMap *hm, char *name, int id) {
    int idx = hash_func(name);
    Bucket *item = (Bucket *)malloc(sizeof(Bucket));
    if (item == NULL) return;
    strncpy(item->name, name, MAX_USERNAME - 1);
    item->name[MAX_USERNAME - 1] = '\0';
    item->user_id = id;
    item->next = hm->slots[idx];
    hm->slots[idx] = item;
}

int hmap_get(HashMap *hm, char *name) {
    int idx = hash_func(name);
    Bucket *cur = hm->slots[idx];
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0)
            return cur->user_id;
        cur = cur->next;
    }
    return -1;
}

void hmap_remove(HashMap *hm, char *name) {
    int idx = hash_func(name);
    Bucket *cur = hm->slots[idx];
    Bucket *prev = NULL;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            if (prev == NULL)
                hm->slots[idx] = cur->next;
            else
                prev->next = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void hmap_free(HashMap *hm) {
    for (int i = 0; i < MAP_SIZE; i++) {
        Bucket *cur = hm->slots[i];
        while (cur != NULL) {
            Bucket *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(hm);
}