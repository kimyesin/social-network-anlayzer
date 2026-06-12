#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// 해시 함수: 각 문자에 위치 가중치(i+1)를 곱해 순서가 다른 문자열이 같은 값이 되는 걸 방지
static int hash_func(char *name) {
    unsigned int result = 0;
    int i = 0;
    while (name[i]) {
        result += (unsigned char)name[i] * (i + 1) * 37;
        i++;
    }
    return (int)(result % MAP_SIZE);
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
            Bucket *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(hm);
}