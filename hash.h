#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 101
#define MAX_USERNAME 50

typedef struct HashEntry {
    char name[MAX_USERNAME];
    int user_id;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry *table[TABLE_SIZE];
} HashTable;

HashTable *hash_create();
void hash_insert(HashTable *ht, char *name, int id);
int hash_search(HashTable *ht, char *name);
void hash_delete(HashTable *ht, char *name);
void hash_free(HashTable *ht);

#endif