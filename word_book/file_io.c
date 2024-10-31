//
// Created by Asaka on 2023/10/26.
//

#include "file_io.h"
#include "stdio.h"
#include "stdlib.h"

STATUS_T file_readln(const char *fileName, int lineNum, char *dst) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }

    int currentLine = 1;
    char ch;
    int pos = 0;
    while ((ch = (char)fgetc(file)) != EOF) {
        if (ch == '\n') {
            currentLine++;
            if (currentLine > lineNum) {
                break;
            }
        } else if (currentLine == lineNum) {
            dst[pos++] = ch;
        }
    }
    dst[pos] = '\0';
    fclose(file);
    if (currentLine < lineNum) {
        return NO_SUCH_LINE;
    }
    return SUCCESS;
}

STATUS_T file_println(const char *fileName, const char *str) {
    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }
    fprintf(file, "%s\n", str);
    fclose(file);

    return SUCCESS;
}

int file_getLineNum(const char * fileName){
    FILE * file;
    file = fopen(fileName, "r");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }
    int lineCount = 0;
    char ch;
    while ((ch = (char)fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }
    if (ch != '\n' && lineCount > 0) {
        lineCount++;
    }
    fclose(file);
    return lineCount;
}

