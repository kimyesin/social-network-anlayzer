#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

ActivityLog *log_create() {
    ActivityLog *log = (ActivityLog *)malloc(sizeof(ActivityLog));
    if (log == NULL) return NULL;
    log->head = NULL;
    log->count = 0;
    return log;
}

// 새 로그를 맨 앞에 추가 (최신순)
void log_add(ActivityLog *log, char *msg) {
    LogNode *node = (LogNode *)malloc(sizeof(LogNode));
    if (node == NULL) return;
    strncpy(node->message, msg, 99);
    node->message[99] = '\0';
    node->next = log->head;
    log->head = node;
    log->count++;
}

void log_print(ActivityLog *log) {
    if (log->head == NULL) {
        printf("No activity log.\n");
        return;
    }
    printf("Activity log (%d entries):\n", log->count);
    LogNode *cur = log->head;
    int i = 1;
    while (cur != NULL) {
        printf("  %d. %s\n", i++, cur->message);
        cur = cur->next;
    }
}

void log_free(ActivityLog *log) {
    LogNode *cur = log->head;
    while (cur != NULL) {
        LogNode *temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(log);
}