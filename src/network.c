#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/network.h"

/* ===================== 해시맵 ===================== */

static int hash_func(char *name) {
    unsigned int h = 0;
    for (int i = 0; name[i]; i++)
        h += (unsigned char)name[i] * (i + 1) * 37;
    return (int)(h % MAP_SIZE);
}

HashMap *hmap_create() {
    HashMap *hm = malloc(sizeof(HashMap));
    if (!hm) return NULL;
    for (int i = 0; i < MAP_SIZE; i++) hm->slots[i] = NULL;
    return hm;
}

void hmap_put(HashMap *hm, char *name, int id) {
    int idx = hash_func(name);
    Bucket *b = malloc(sizeof(Bucket));
    if (!b) return;
    strncpy(b->name, name, MAX_USERNAME - 1);
    b->name[MAX_USERNAME - 1] = '\0';
    b->user_id = id;
    b->next = hm->slots[idx];
    hm->slots[idx] = b;
}

int hmap_get(HashMap *hm, char *name) {
    Bucket *cur = hm->slots[hash_func(name)];
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur->user_id;
        cur = cur->next;
    }
    return -1;
}

void hmap_remove(HashMap *hm, char *name) {
    int idx = hash_func(name);
    Bucket *cur = hm->slots[idx], *prev = NULL;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            if (prev) prev->next = cur->next;
            else hm->slots[idx] = cur->next;
            free(cur);
            return;
        }
        prev = cur; cur = cur->next;
    }
}

void hmap_free(HashMap *hm) {
    for (int i = 0; i < MAP_SIZE; i++) {
        Bucket *cur = hm->slots[i];
        while (cur) { Bucket *t = cur; cur = cur->next; free(t); }
    }
    free(hm);
}

/* ===================== 그래프 ===================== */

static int are_friends(Graph *g, int a, int b) {
    AdjNode *cur = g->users[a].head;
    while (cur) { if (cur->dest == b) return 1; cur = cur->next; }
    return 0;
}

static void add_edge(Graph *g, int from, int to) {
    AdjNode *node = malloc(sizeof(AdjNode));
    if (!node) return;
    node->dest = to;
    node->next = g->users[from].head;
    g->users[from].head = node;
}

Graph *graph_create() {
    Graph *g = malloc(sizeof(Graph));
    if (!g) return NULL;
    g->user_count = 0;
    g->hm = hmap_create();
    for (int i = 0; i < MAX_USERS; i++) {
        g->users[i].is_active = 0;
        g->users[i].head = NULL;
    }
    return g;
}

void graph_free(Graph *g) {
    for (int i = 0; i < MAX_USERS; i++) {
        AdjNode *cur = g->users[i].head;
        while (cur) { AdjNode *t = cur; cur = cur->next; free(t); }
    }
    hmap_free(g->hm);
    free(g);
}

void graph_add_user(Graph *g, char *name) {
    if (hmap_get(g->hm, name) != -1) {
        printf("%s already exists.\n", name); return;
    }
    int id = -1;
    for (int i = 0; i < MAX_USERS; i++)
        if (!g->users[i].is_active) { id = i; break; }
    if (id == -1) { printf("Max users reached.\n"); return; }

    g->users[id].id = id;
    strncpy(g->users[id].name, name, MAX_USERNAME - 1);
    g->users[id].name[MAX_USERNAME - 1] = '\0';
    g->users[id].head = NULL;
    g->users[id].is_active = 1;
    g->user_count++;
    hmap_put(g->hm, name, id);
    printf("User '%s' created.\n", name);
}

void graph_remove_user(Graph *g, char *name) {
    int id = hmap_get(g->hm, name);
    if (id == -1) { printf("User '%s' not found.\n", name); return; }

    AdjNode *cur = g->users[id].head;
    while (cur) { AdjNode *t = cur; cur = cur->next; free(t); }
    g->users[id].head = NULL;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active || i == id) continue;
        AdjNode *p = g->users[i].head, *prev = NULL;
        while (p) {
            if (p->dest == id) {
                if (prev) prev->next = p->next;
                else g->users[i].head = p->next;
                free(p); break;
            }
            prev = p; p = p->next;
        }
    }
    g->users[id].is_active = 0;
    g->user_count--;
    hmap_remove(g->hm, name);
    printf("User '%s' deleted.\n", name);
}

void graph_add_friend(Graph *g, char *u1, char *u2) {
    int a = hmap_get(g->hm, u1), b = hmap_get(g->hm, u2);
    if (a == -1 || b == -1) { printf("User not found.\n"); return; }
    if (are_friends(g, a, b)) {
        printf("%s and %s are already friends.\n", u1, u2); return;
    }
    add_edge(g, a, b);
    add_edge(g, b, a);
    printf("%s and %s are now friends.\n", u1, u2);
}

void graph_remove_friend(Graph *g, char *u1, char *u2) {
    int a = hmap_get(g->hm, u1), b = hmap_get(g->hm, u2);
    if (a == -1 || b == -1) return;

    AdjNode *cur = g->users[a].head, *prev = NULL;
    while (cur) {
        if (cur->dest == b) {
            if (prev) prev->next = cur->next;
            else g->users[a].head = cur->next;
            free(cur); break;
        }
        prev = cur; cur = cur->next;
    }
    cur = g->users[b].head; prev = NULL;
    while (cur) {
        if (cur->dest == a) {
            if (prev) prev->next = cur->next;
            else g->users[b].head = cur->next;
            free(cur); break;
        }
        prev = cur; cur = cur->next;
    }
    printf("Removed friendship: %s <-> %s.\n", u1, u2);
}

void graph_print_friends(Graph *g, char *name) {
    int id = hmap_get(g->hm, name);
    if (id == -1) { printf("User '%s' not found.\n", name); return; }
    AdjNode *cur = g->users[id].head;
    if (!cur) { printf("%s has no friends yet.\n", name); return; }
    printf("%s's friends:\n", name);
    int idx = 1;
    while (cur) {
        printf("  %d. %s\n", idx++, g->users[cur->dest].name);
        cur = cur->next;
    }
}

/* ===================== BFS ===================== */

int bfs_distance(Graph *g, char *start, char *target) {
    int src = hmap_get(g->hm, start);
    int dst = hmap_get(g->hm, target);
    if (src == -1 || dst == -1) { printf("User not found.\n"); return -1; }
    if (src == dst) return 0;

    int q[MAX_USERS], depth[MAX_USERS], seen[MAX_USERS];
    memset(seen,  0, sizeof(seen));
    memset(depth, 0, sizeof(depth));

    int head = 0, tail = 0;
    seen[src] = 1;
    q[tail++] = src;

    while (head < tail) {
        int node = q[head++];
        AdjNode *nb = g->users[node].head;
        while (nb) {
            int nxt = nb->dest;
            if (!seen[nxt]) {
                seen[nxt]  = 1;
                depth[nxt] = depth[node] + 1;
                if (nxt == dst) return depth[nxt];
                q[tail++] = nxt;
            }
            nb = nb->next;
        }
    }
    return -1;
}

/* ===================== DFS (명시적 스택) ===================== */

void dfs_find_groups(Graph *g) {
    int visited[MAX_USERS] = {0};
    int stack[MAX_USERS];
    int group_num = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active || visited[i]) continue;

        group_num++;
        printf("Group %d: ", group_num);

        int top = 0;
        visited[i] = 1;
        stack[top++] = i;

        while (top > 0) {
            int cur = stack[--top];
            printf("%s ", g->users[cur].name);
            AdjNode *nb = g->users[cur].head;
            while (nb) {
                if (!visited[nb->dest]) {
                    visited[nb->dest] = 1;
                    stack[top++] = nb->dest;
                }
                nb = nb->next;
            }
        }
        printf("\n");
    }
    printf("%d group(s) found\n", group_num);
}