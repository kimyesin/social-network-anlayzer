#ifndef FEATURE_H
#define FEATURE_H

#include "network.h"

/* --------- 친구 추천 --------- */
void recommend_friends(Graph *g, char *name);

/* --------- 영향력 분석 --------- */
void centrality_top_k(Graph *g, int k);

/* --------- 활동 로그 --------- */
typedef struct LogNode {
    char message[100];
    struct LogNode *next;
} LogNode;

typedef struct {
    LogNode *head;
    int count;
} ActivityLog;

ActivityLog *log_create();
void         log_add(ActivityLog *log, char *msg);
void         log_print(ActivityLog *log);
void         log_free(ActivityLog *log);

#endif