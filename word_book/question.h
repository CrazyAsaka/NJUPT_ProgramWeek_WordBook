//
// Created by Asaka on 2023/10/31.
//

#ifndef WORD_BOOK_QUESTION_H
#define WORD_BOOK_QUESTION_H

#define CHOICE_QUESTION_

#include "stdint.h"
#include "word.h"
#include "stdlib.h"
#include "time.h"
#include "status.h"
#include "string.h"

#define QUESTION_LEN 50

#ifdef CHOICE_QUESTION_
#define ONE_SELECTION_LEN 20
#define SELECTION_NUMS 4
#endif

typedef uint8_t questSize_t;

typedef struct question_t {
    char questTitle[QUESTION_LEN];          /*The title of Question*/
    wordInfo * questWord;
    questSize_t questSeqNum;                /*The sequence number of the question*/
#ifdef CHOICE_QUESTION_
    wordInfo * selectionWords[SELECTION_NUMS];     /*Selection Description*/
    questSize_t selectedAnsIndex;
    questSize_t corrAnsIndex;
#endif
    struct question_t * next;
}question_t;

void quest_show (void);
void quest_srand (void);
STATUS_T quest_create (const char * sourceFile, questSize_t questNum);
STATUS_T quest_create_errword(const char * sourceFile, questSize_t _questNum);

#endif //WORD_BOOK_QUESTION_H
