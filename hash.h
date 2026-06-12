#ifndef HASH_H
#define HASH_H

#define MAP_SIZE 97
#define MAX_USERNAME 50

typedef struct Bucket {
    char name[MAX_USERNAME];
    int user_id;
    struct Bucket *next;
} Bucket;

typedef struct {
    Bucket *slots[MAP_SIZE];
} HashMap;

HashMap *hmap_create();
void hmap_put(HashMap *hm, char *name, int id);
int  hmap_get(HashMap *hm, char *name);
void hmap_remove(HashMap *hm, char *name);
void hmap_free(HashMap *hm);

#endif