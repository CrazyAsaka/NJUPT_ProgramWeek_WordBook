//
// Created by Asaka on 2023/10/27.
//
#include "word.h"
wordInfo * wordList = NULL;
errWordInfo * errWordList = NULL;
extern userInfo * curUsr;
FILE_PRINTLN f_println = file_println;
FILE_READLN freadln = file_readln;     /*Connect file_io with user */
wordSize_t wordTotalNum;    /*count word numbers*/
wordSize_t errWordTotalNum;

int deleteLineFromFile(const char *filename, int lineToDelete) {
    FILE *file, *tempFile;
    char ch;
    int line = 1;

    file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        fclose(file);
        return 1;
    }

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            line++;
        }
        if (line != lineToDelete) {
            fputc(ch, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove(filename) != 0) {
        return 1;
    }

    if (rename("temp.txt", filename) != 0) {
        return 1;
    }

    return 0;
}

void word_clear(void) {
    wordInfo * current = wordList;
    wordInfo * next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    wordList = NULL;
}

STATUS_T word_init (void) {
    STATUS_T status;
    status = word_readFile(WORD_FILE);
    return status;
}

STATUS_T word_add (const char * word, const char * part,const char * meaning) {
    if (word_find(word) == NULL) {
        wordInfo* newWord = (wordInfo*)malloc(sizeof(wordInfo));
        if (newWord == NULL) {
            return MEMORY_ERROR;
        }
        newWord->sign = unctn;
        newWord->wordSeqNum = wordTotalNum++;
        strcpy(newWord->word, word);
        strcpy(newWord->part, part);
        strcpy(newWord->meaning, meaning);
        newWord->next = wordList;
        wordList = newWord;
        word_saveFile(WORD_FILE);
        return SUCCESS;
    }
    return WORD_EXIST;
//    char line[MEAN_LEN+WORD_LEN+4];
//    sprintf(line,"%d %s %s %s",
//            newWord->sign, newWord->word, newWord->part, newWord->meaning);
//    f_println(WORD_FILE,line);
}

wordInfo * word_getWord (wordSize_t seqNum) {
    wordInfo * wp = wordList;
    while (wp != NULL) {
        if (wp->wordSeqNum == seqNum) {
            return wp;
        }
        wp = wp->next;
    }
    return NULL;
}

wordInfo* word_find(const char* word) {
    wordInfo* current = wordList;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
//            printf("%s\n", current->meaning);
            return current;
        }
        current = current->next;
    }
    return NULL; /*No word*/
}

STATUS_T word_delete (const char * word) {
    wordInfo* current = wordList;
    wordInfo* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {   /*Match SUCCESS*/
            if (prev == NULL) {
                wordList = current->next;
            } else {
                prev->next = current->next;
            }
            word_saveFile(WORD_FILE);
            free(current);
            return SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    word_saveFile(WORD_FILE);
    return NO_WORD;
}

STATUS_T word_extractLn (const char * line) {
    int sign;
    char word[WORD_LEN] = {0};
    char part[PART_LEN] = {0};
    char meaning[MEAN_LEN] = {0};
    if(sscanf(line, "%d %s %s %s", &sign, word, part, meaning) == 4){
        wordInfo* newWord = (wordInfo*)malloc(sizeof(wordInfo));
        if (newWord == NULL) {
            return MEMORY_ERROR;
        }
        newWord->sign = sign;
        newWord->wordSeqNum = wordTotalNum++;
        strcpy(newWord->word, word);
        strcpy(newWord->part, part);
        strcpy(newWord->meaning, meaning);
        newWord->next = wordList;
        wordList = newWord;
        return SUCCESS;
    }
    return USER_EXTRACT_ERROR;
}

STATUS_T word_eng2cn (const char * word, char * dst) {
    wordInfo * wp = word_find(word);
    if (wp == NULL) {
        return NO_WORD;
    }
    strcpy(dst, wp->meaning);
    return SUCCESS;
}

STATUS_T word_cn2eng (const char * meaning, char * dst) {
    wordInfo* current = wordList;
    while (current != NULL) {
        if (strcmp(current->meaning, meaning) == 0) {
            strcpy(dst, current->word);
            return SUCCESS;
        }
        current = current->next;
    }
    /*No such word*/
    return NO_WORD;
}

void word_showAll (void) {
    wordInfo * wp = wordList;
    while(wp != NULL){
        printf("%s    %s    %s\n",wp->word, wp->part, wp->meaning);
        wp = wp->next;
    }
}

STATUS_T word_edit (const char * word, const char * newPart, const char * newMeaning) {
    wordInfo * wp = word_find(word);
    if (wp == NULL) {
        return NO_WORD;
    }
    strcpy(wp->part, newPart);
    strcpy(wp->meaning, newMeaning);
    word_saveFile(WORD_FILE);
    return SUCCESS;
}

STATUS_T word_saveFile (const char * fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }

    wordInfo * wp = wordList;
    while (wp != NULL) {
        fprintf(file, "%d %s %s %s\n",
                 wp->sign, wp->word, wp->part, wp->meaning);
        wp = wp->next;
    }
    fclose(file);
    return SUCCESS;
}

STATUS_T word_readFile (const char * fileName) {
    STATUS_T status;
    int lineCnt = file_getLineNum(fileName);
    char fileLine[WORD_LEN + MEAN_LEN + PART_LEN + 4] = {0};
    for(int line = 1; line <= lineCnt; line++){
        status = freadln(fileName, line, fileLine);
        word_extractLn(fileLine);
    }
    return status;
}

/*
 * Error word function:
 * */

/*
 * @attention:  This function MUST BE CALLED AFTER USER LOGGING IN!!!
 * */
STATUS_T errword_init (void) {
    STATUS_T status;
    status = errword_readFile(ERRWORD_FILE);
    return status;
}

STATUS_T errword_add (const char * word) {
    wordInfo * wd = word_find(word);
    if (wd == NULL) {
        return NO_WORD;
    }
    if (errword_find(word) != NULL) {
        return WORD_EXIST;
    }
    wd->sign = errWord;
    errWordInfo* newErrWord = (errWordInfo*)malloc(sizeof(errWordInfo));
    if (newErrWord == NULL) {
        return MEMORY_ERROR;
    }
    newErrWord->wordSeqNum = errWordTotalNum++;
    strcpy(newErrWord->word, word);
    strcpy(newErrWord->part, wd->part);
    strcpy(newErrWord->meaning, wd->meaning);
    newErrWord->next = errWordList;
    errWordList = newErrWord;
    char line[MEAN_LEN+WORD_LEN+4];
    sprintf(line,"%s %s %s %s",
            curUsr->username, wd->word, wd->part, wd->meaning);
    f_println(ERRWORD_FILE,line);
    return SUCCESS;
}

STATUS_T errword_edit (const char * errword, const char * newPart, const char * newMeaning) {
    errWordInfo * wp = errword_find(errword);
    if (wp == NULL) {
        return NO_WORD;
    }
    char line[PART_LEN+MEAN_LEN+ID_LEN+WORD_LEN + 10];
    strcpy(wp->part, newPart);
    strcpy(wp->meaning, newMeaning);
    deleteLineFromFile(ERRWORD_FILE, errword_getLine(wp->word));
    sprintf(line, "%s %s %s %s",
                 curUsr->username,
                 wp->word,
                 wp->part,
                 wp->meaning);
    file_println(ERRWORD_FILE, line);
    return SUCCESS;
}

void errword_resort (void) {
    errWordInfo * wp = errWordList;
    errWordTotalNum = 0;
    while(wp != NULL){
        wp->wordSeqNum = errWordTotalNum++;
        wp = wp->next;
    }
}

STATUS_T errword_delete (const char * errword) {
    errWordInfo* current = errWordList;
    errWordInfo* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->word, errword) == 0) {   /*Match SUCCESS*/
            int line = errword_getLine(errword);
            deleteLineFromFile(ERRWORD_FILE, line);

            if (prev == NULL) {
                errWordList = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            errword_resort();
            return SUCCESS;
        }
        prev = current;
        current = current->next;
    }

    return NO_WORD;
}


int errword_getLine (const char * word) {
    char line[MEAN_LEN + ID_LEN + PART_LEN + WORD_LEN + 10];
    char usrName[ID_LEN];
    char word_[WORD_LEN];
    char _[MEAN_LEN];
    char __[PART_LEN];

    int lineCnt = file_getLineNum(ERRWORD_FILE);
    for (int index = 1; index <=lineCnt; index++) {
        file_readln(ERRWORD_FILE, index, line);
        sscanf(line, "%s %s %s %s", usrName, word_, __, _);
        if (strcmp(usrName, curUsr->username) == 0) {
            if (strcmp(word_, word) == 0) {
                return index;
            }
        }
    }
    return 0;
}

void errword_showAll (void) {
    errWordInfo * wp = errWordList;
    while(wp != NULL){
        printf("%s\t\t%s\t%s\n",wp->word, wp->part, wp->meaning);
        wp = wp->next;
    }
}

wordInfo * errword_getWord (wordSize_t seqNum) {
    errWordInfo * wp = errWordList;
    while (wp != NULL) {
        if (wp->wordSeqNum == seqNum) {
            return wp;
        }
        wp = wp->next;
    }
    return NULL;
}

errWordInfo * errword_find (const char * errWord) {
    errWordInfo* current = errWordList;
    while (current != NULL) {
        if (strcmp(current->word, errWord) == 0) {
//            printf("%s\n", current->meaning);
            return current;
        }
        current = current->next;
    }
    return NULL; /*No user*/
}

STATUS_T errword_saveFile (const char * fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }

    errWordInfo * wp = errWordList;
    while (wp != NULL) {
        fprintf(file, "%s %s %s %s\n",
                curUsr->username, wp->word, wp->part, wp->meaning);
        wp = wp->next;
    }
    fclose(file);
    return SUCCESS;
}

STATUS_T errword_readFile (const char * fileName) {
    STATUS_T status;
    errword_clear();
    int lineCnt = file_getLineNum(fileName);
    char fileLine[WORD_LEN + MEAN_LEN + PART_LEN + 4] = {0};
    for(int line = 1; line <= lineCnt; line++){
        status = freadln(ERRWORD_FILE, line, fileLine);
        errword_extractLn(fileLine);
    }
    return status;
}

STATUS_T errword_extractLn (const char * line) {
    char word[WORD_LEN] = {0};
    char part[PART_LEN] = {0};
    char meaning[MEAN_LEN] = {0};
    char username[ID_LEN] = {0};
    if((sscanf(line, "%s %s %s %s", username, word, part, meaning) == 4)
        && (strcmp(username, curUsr->username) == 0)){
        errWordInfo* newErrWord = (errWordInfo*)malloc(sizeof(errWordInfo));
        if (newErrWord == NULL) {
            return MEMORY_ERROR;
        }
        strcpy(newErrWord->word, word);
        strcpy(newErrWord->part, part);
        strcpy(newErrWord->meaning, meaning);
        newErrWord->sign = unquested;
        newErrWord->wordSeqNum = errWordTotalNum++;
        newErrWord->next = errWordList;
        errWordList = newErrWord;
        return SUCCESS;
    }
    return USER_EXTRACT_ERROR;
}

void errword_clear (void) {
    errWordList = NULL;
    errWordTotalNum = 0;
}
