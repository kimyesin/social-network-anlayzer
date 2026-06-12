#ifndef SORT_H
#define SORT_H

#include "graph.h"

// 친구 추천 후보 (공통 친구 수와 이름 저장)
typedef struct {
    int user_id;
    char name[MAX_USERNAME];
    int common_count;  // 공통 친구 수
} Candidate;

// 친구 추천 (공통 친구 수 기반, 내림차순 출력)
void recommend_friends(Graph *g, char *name);

#endif