#ifndef LOG_H
#define LOG_H

#include "graph.h"

// 활동 로그 한 개
typedef struct LogNode {
    char message[100];
    struct LogNode *next;
} LogNode;

// 로그 전체 (연결리스트)
typedef struct {
    LogNode *head;
    int count;
} ActivityLog;

// 함수
ActivityLog *log_create();
void log_add(ActivityLog *log, char *msg);
void log_print(ActivityLog *log);
void log_free(ActivityLog *log);

#endif