//
// Created by Asaka on 2023/10/26.
//
#ifndef _STATUS_H
#define _STATUS_H

typedef enum {
    FAILURE = 0,
    SUCCESS,
    END,
    CONTINUE,
    NO_SUCH_LINE,
    TYPE_ERROR,     /*PROGRAM STATUS*/
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR,
    FILE_READ_ERROR,
    MEMORY_ERROR,
    NO_USER_NAME,   /*USER STATUS*/
    USER_EXTRACT_ERROR,
    PSWD_ERROR,
    USER_EXIST,
    USER_AVAILABLE,
    NO_WORD,
    WORD_EXIST,
    QUEST_NUM_OVERFLOW
}STATUS_T;


#endif //_STATUS_H
