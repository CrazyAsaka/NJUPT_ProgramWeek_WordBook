//
// Created by Asaka on 2023/10/27.
//
#ifndef WORD_BOOK_WORD_H
#define WORD_BOOK_WORD_H

#include "status.h"
#include "stdio.h"
#include "stdlib.h"
#include "file_io.h"
#include "string.h"
#include "stdint.h"
#include "user.h"

#define WORD_LEN 15
#define MEAN_LEN 40
#define PART_LEN 15

typedef uint32_t wordSize_t;

typedef enum {
    quested = 0,
    unquested,
    errWord,
    corrWord,
    unctn
}wordSign;

typedef struct wordInfo{
    wordSign sign;
    wordSize_t wordSeqNum;
    char word[WORD_LEN];
    char part[PART_LEN];
    wchar_t meaning[MEAN_LEN];
    struct wordInfo * next;
}wordInfo;

typedef struct errWordInfo{
    wordSign sign;
    wordSize_t wordSeqNum;
    char word[WORD_LEN];
    char part[PART_LEN];
    char meaning[MEAN_LEN];
    struct errWordInfo * next;
}errWordInfo;

typedef STATUS_T    (*FILE_PRINTLN)     (const char *fileName, const char *str);
typedef STATUS_T    (*FILE_READLN)      (const char *fileName, int lineNum, char *dst); /* Declare a function pointer */

int deleteLineFromFile(const char *filename, int lineToDelete);

void word_clear (void);
void word_showAll (void);
STATUS_T     word_init           (void);
STATUS_T     word_add            (const char * word, const char * part,const char * meaning);
STATUS_T     word_edit           (const char * word, const char * newPart, const char * newMeaning);
STATUS_T     word_delete         (const char * word);
STATUS_T     word_extractLn      (const char * line);
STATUS_T     word_saveFile       (const char * fileName);
STATUS_T     word_readFile       (const char * fileName);
STATUS_T     word_eng2cn         (const char * word, char * dst);
STATUS_T     word_cn2eng         (const char * meaning, char * dst);
wordInfo  *  word_find           (const char * word);
wordInfo  *  word_getWord        (wordSize_t seqNum);

wordInfo * errword_getWord (wordSize_t seqNum);
void errword_resort (void);
int errword_getLine (const char * word);
int errword_getLine (const char * word);
void errword_clear (void);
void errword_showAll (void);
STATUS_T errword_init (void);
STATUS_T errword_add (const char * word);
STATUS_T errword_delete (const char * errword);
STATUS_T errword_extractLn (const char * line);
STATUS_T errword_saveFile (const char * fileName);
STATUS_T errword_readFile (const char * fileName);
STATUS_T errword_edit (const char * errword, const char * newPart, const char * newMeaning);
errWordInfo *errword_find (const char * errWord);

#endif //WORD_BOOK_WORD_H
