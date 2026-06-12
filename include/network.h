#ifndef NETWORK_H
#define NETWORK_H

#define MAX_USERS    100
#define MAX_USERNAME  50
#define MAP_SIZE      97

/* --------- 해시맵 --------- */
typedef struct Bucket {
    char name[MAX_USERNAME];
    int  user_id;
    struct Bucket *next;
} Bucket;

typedef struct {
    Bucket *slots[MAP_SIZE];
} HashMap;

HashMap *hmap_create();
void     hmap_put(HashMap *hm, char *name, int id);
int      hmap_get(HashMap *hm, char *name);
void     hmap_remove(HashMap *hm, char *name);
void     hmap_free(HashMap *hm);

/* --------- 그래프 --------- */
typedef struct AdjNode {
    int dest;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    int  id;
    char name[MAX_USERNAME];
    AdjNode *head;
    int  is_active;
} User;

typedef struct {
    User    users[MAX_USERS];
    int     user_count;
    HashMap *hm;
} Graph;

Graph *graph_create();
void   graph_free(Graph *g);
void   graph_add_user(Graph *g, char *name);
void   graph_remove_user(Graph *g, char *name);
void   graph_add_friend(Graph *g, char *u1, char *u2);
void   graph_remove_friend(Graph *g, char *u1, char *u2);
void   graph_print_friends(Graph *g, char *name);

/* --------- 탐색 (BFS / DFS) --------- */
int  bfs_distance(Graph *g, char *start, char *target);
void dfs_find_groups(Graph *g);

#endif