#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

static int are_connected(Graph *g, int id_a, int id_b) {
    AdjNode *cur = g->users[id_a].head;
    while (cur != NULL) {
        if (cur->dest == id_b) return 1;
        cur = cur->next;
    }
    return 0;
}

Graph *graph_create() {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (g == NULL) return NULL;
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
        while (cur != NULL) {
            AdjNode *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    hmap_free(g->hm);
    free(g);
}

void graph_add_user(Graph *g, char *name) {
    if (hmap_get(g->hm, name) != -1) {
        printf("%s already exists.\n", name);
        return;
    }
    int id = -1;
    for (int i = 0; i < MAX_USERS; i++) {
        if (g->users[i].is_active == 0) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        printf("Max users reached!\n");
        return;
    }
    g->users[id].id = id;
    strncpy(g->users[id].name, name, MAX_USERNAME - 1);
    g->users[id].name[MAX_USERNAME - 1] = '\0';
    g->users[id].head = NULL;
    g->users[id].is_active = 1;
    g->user_count++;
    hmap_put(g->hm, name, id);
    printf("%s created.\n", name);
}

void graph_remove_user(Graph *g, char *name) {
    int id = hmap_get(g->hm, name);
    if (id == -1) {
        printf("User '%s' not found.\n", name);
        return;
    }
    AdjNode *cur = g->users[id].head;
    while (cur != NULL) {
        AdjNode *temp = cur;
        cur = cur->next;
        free(temp);
    }
    g->users[id].head = NULL;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active || i == id) continue;
        AdjNode *cur = g->users[i].head;
        AdjNode *prev = NULL;
        while (cur != NULL) {
            if (cur->dest == id) {
                if (prev == NULL)
                    g->users[i].head = cur->next;
                else
                    prev->next = cur->next;
                free(cur);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    g->users[id].is_active = 0;
    g->user_count--;
    hmap_remove(g->hm, name);
    printf("%s deleted.\n", name);
}

void graph_add_friend(Graph *g, char *user1, char *user2) {
    int id_a = hmap_get(g->hm, user1);
    int id_b = hmap_get(g->hm, user2);
    if (id_a == -1 || id_b == -1) {
        printf("User not found.\n");
        return;
    }
    if (are_connected(g, id_a, id_b)) {
        printf("%s and %s are already friends.\n", user1, user2);
        return;
    }
    AdjNode *link_to_b = (AdjNode *)malloc(sizeof(AdjNode));
    if (link_to_b == NULL) return;
    link_to_b->dest = id_b;
    link_to_b->next = g->users[id_a].head;
    g->users[id_a].head = link_to_b;

    AdjNode *link_to_a = (AdjNode *)malloc(sizeof(AdjNode));
    if (link_to_a == NULL) return;
    link_to_a->dest = id_a;
    link_to_a->next = g->users[id_b].head;
    g->users[id_b].head = link_to_a;

    printf("%s and %s are now friends.\n", user1, user2);
}

void graph_remove_friend(Graph *g, char *user1, char *user2) {
    int id_a = hmap_get(g->hm, user1);
    int id_b = hmap_get(g->hm, user2);
    if (id_a == -1 || id_b == -1) return;

    AdjNode *cur = g->users[id_a].head;
    AdjNode *prev = NULL;
    while (cur != NULL) {
        if (cur->dest == id_b) {
            if (prev == NULL)
                g->users[id_a].head = cur->next;
            else
                prev->next = cur->next;
            free(cur);
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    cur = g->users[id_b].head;
    prev = NULL;
    while (cur != NULL) {
        if (cur->dest == id_a) {
            if (prev == NULL)
                g->users[id_b].head = cur->next;
            else
                prev->next = cur->next;
            free(cur);
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("Removed friendship: %s and %s.\n", user1, user2);
}

void graph_print_friends(Graph *g, char *name) {
    int id = hmap_get(g->hm, name);
    if (id == -1) {
        printf("User '%s' not found.\n", name);
        return;
    }
    AdjNode *cur = g->users[id].head;
    if (cur == NULL) {
        printf("%s has no connections yet.\n", name);
        return;
    }
    printf("%s's connections:\n", name);
    int idx = 1;
    while (cur != NULL) {
        printf("  %d. %s\n", idx++, g->users[cur->dest].name);
        cur = cur->next;
    }
}