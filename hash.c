#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* 이름 문자열을 숫자로 변환하는 함수
   각 글자의 ASCII 값에 31을 곱하면서 누적
   31을 쓰는 이유: 소수라서 글자 순서가 달라도 결과가 달라짐 */
static int hash_func(char *name) {
    int result = 0;
    while (*name) {              // 문자열 끝(\0)까지 반복
        result = result * 31 + *name;  // 현재까지 값 * 31 + 이번 글자
        name++;                  // 다음 글자로 이동
    }
    // 음수 방지 후 TABLE_SIZE로 나눈 나머지 = 테이블 인덱스
    return (result % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
}

/* 해시 테이블 생성
   모든 칸을 NULL로 초기화 (아무것도 없는 빈 테이블) */
HashTable *hash_create() {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;     // 각 칸 비어있음
    }
    return ht;
}

/* 이름과 ID를 테이블에 저장
   같은 칸에 이미 뭔가 있으면 앞에 끼워넣기 (체이닝) */
void hash_insert(HashTable *ht, char *name, int id) {
    int idx = hash_func(name);   // 이름 → 인덱스 계산

    // 새 항목 만들기
    HashEntry *item = (HashEntry *)malloc(sizeof(HashEntry));
    strncpy(item->name, name, MAX_USERNAME - 1);  // 이름 복사
    item->name[MAX_USERNAME - 1] = '\0';          // 문자열 끝 보장
    item->user_id = id;

    // 해당 칸 맨 앞에 끼워넣기
    item->next = ht->table[idx];
    ht->table[idx] = item;
}

/* 이름으로 ID 찾기
   못 찾으면 -1 반환 */
int hash_search(HashTable *ht, char *name) {
    int idx = hash_func(name);       // 어느 칸인지 계산
    HashEntry *cur = ht->table[idx]; // 그 칸의 첫 항목부터 시작

    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0)  // 이름 일치하면
            return cur->user_id;           // ID 반환
        cur = cur->next;                   // 다음 항목으로
    }
    return -1;  // 끝까지 못 찾음
}

/* 이름에 해당하는 항목 삭제 */
void hash_delete(HashTable *ht, char *name) {
    int idx = hash_func(name);
    HashEntry *cur = ht->table[idx];
    HashEntry *prev = NULL;  // 이전 항목 기억 (연결 끊을 때 필요)

    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            if (prev == NULL)
                ht->table[idx] = cur->next;  // 첫 번째 항목 삭제
            else
                prev->next = cur->next;      // 중간/끝 항목 삭제
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

/* 테이블 전체 메모리 해제
   프로그램 종료 시 호출 */
void hash_free(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry *cur = ht->table[i];
        while (cur != NULL) {
            HashEntry *temp = cur;
            cur = cur->next;
            free(temp);          // 하나씩 해제
        }
    }
    free(ht);
}