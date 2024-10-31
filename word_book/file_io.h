//
// Created by Asaka on 2023/10/26.
//

#ifndef WORD_BOOK_FILE_IO_H
#define WORD_BOOK_FILE_IO_H
#include "status.h"

#define     USER_FILE       "user.txt"
#define     WORD_FILE       "wordbook.txt"
#define     ERRWORD_FILE    "errWordbook.txt"

//const char * WORD_FILE = "wordbook.txt";

STATUS_T file_println       (const char * fileName, const char * str);
STATUS_T file_readln        (const char * fileName, int lineNum, char * dst);
int      file_getLineNum    (const char * fileName);

#endif //WORD_BOOK_FILE_IO_H
