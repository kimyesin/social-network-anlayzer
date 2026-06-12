#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/feature.h"

/* ===================== 친구 추천 ===================== */

typedef struct {
    int  user_id;
    char name[MAX_USERNAME];
    int  common_count;
} Candidate;

// 삽입 정렬 내림차순
static void insertion_sort_desc(Candidate *arr, int n) {
    for (int i = 1; i < n; i++) {
        Candidate key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].common_count < key.common_count) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void recommend_friends(Graph *g, char *name) {
    int id = hmap_get(g->hm, name);
    if (id == -1) { printf("User not found: %s\n", name); return; }

    Candidate cands[MAX_USERS];
    int cnt = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active || i == id) continue;

        // 이미 친구면 스킵
        int already = 0;
        AdjNode *chk = g->users[id].head;
        while (chk) { if (chk->dest == i) { already = 1; break; } chk = chk->next; }
        if (already) continue;

        // 공통 친구 수 계산
        int common = 0;
        AdjNode *a = g->users[id].head;
        while (a) {
            AdjNode *b = g->users[i].head;
            while (b) { if (a->dest == b->dest) common++; b = b->next; }
            a = a->next;
        }
        if (common > 0) {
            cands[cnt].user_id = i;
            strncpy(cands[cnt].name, g->users[i].name, MAX_USERNAME - 1);
            cands[cnt].common_count = common;
            cnt++;
        }
    }

    insertion_sort_desc(cands, cnt);

    printf("Friend recommendations for %s:\n", name);
    if (cnt == 0) { printf("  No recommendations.\n"); return; }
    for (int i = 0; i < cnt; i++)
        printf("  %s (common friends: %d)\n", cands[i].name, cands[i].common_count);
}

/* ===================== 영향력 분석 ===================== */

typedef struct {
    int  user_id;
    char name[MAX_USERNAME];
    int  degree;
} DegreeInfo;

static int count_degree(Graph *g, int id) {
    int n = 0;
    AdjNode *cur = g->users[id].head;
    while (cur) { n++; cur = cur->next; }
    return n;
}

// 선택 정렬 내림차순
static void selection_sort_desc(DegreeInfo *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].degree > arr[max_idx].degree) max_idx = j;
        if (max_idx != i) {
            DegreeInfo t = arr[i]; arr[i] = arr[max_idx]; arr[max_idx] = t;
        }
    }
}

void centrality_top_k(Graph *g, int k) {
    DegreeInfo info[MAX_USERS];
    int cnt = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active) continue;
        info[cnt].user_id = i;
        strncpy(info[cnt].name, g->users[i].name, MAX_USERNAME - 1);
        info[cnt].degree = count_degree(g, i);
        cnt++;
    }
    selection_sort_desc(info, cnt);
    if (k > cnt) k = cnt;
    printf("Top %d influential users:\n", k);
    for (int i = 0; i < k; i++)
        printf("  %d. %s (friends: %d)\n", i + 1, info[i].name, info[i].degree);
}

/* ===================== 활동 로그 ===================== */

ActivityLog *log_create() {
    ActivityLog *log = malloc(sizeof(ActivityLog));
    if (!log) return NULL;
    log->head = NULL;
    log->count = 0;
    return log;
}

// 최신순으로 맨 앞에 추가
void log_add(ActivityLog *log, char *msg) {
    LogNode *node = malloc(sizeof(LogNode));
    if (!node) return;
    strncpy(node->message, msg, 99);
    node->message[99] = '\0';
    node->next = log->head;
    log->head = node;
    log->count++;
}

void log_print(ActivityLog *log) {
    if (!log->head) { printf("No activity log.\n"); return; }
    printf("Activity log (%d entries):\n", log->count);
    LogNode *cur = log->head;
    int i = 1;
    while (cur) {
        printf("  %d. %s\n", i++, cur->message);
        cur = cur->next;
    }
}

void log_free(ActivityLog *log) {
    LogNode *cur = log->head;
    while (cur) { LogNode *t = cur; cur = cur->next; free(t); }
    free(log);
}