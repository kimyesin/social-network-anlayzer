#ifndef GRAPH_H
#define GRAPH_H

#include "hash.h"

#define MAX_USERS 100

typedef struct AdjNode {
    int dest;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    int id;
    char name[MAX_USERNAME];
    AdjNode *head;
    int is_active;
} User;

typedef struct {
    User users[MAX_USERS];
    int user_count;
    HashTable *ht;
} Graph;

// 함수 선언
Graph *graph_create();
void graph_add_user(Graph *g, char *name);
void graph_remove_user(Graph *g, char *name);
void graph_add_friend(Graph *g, char *a, char *b);
void graph_remove_friend(Graph *g, char *a, char *b);
void graph_print_friends(Graph *g, char *name);
void graph_free(Graph *g);

#endif