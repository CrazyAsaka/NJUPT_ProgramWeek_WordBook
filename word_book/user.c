//
// Created by Asaka on 2023/10/26.
//

#include "user.h"
#include "stdlib.h"
#include "string.h"
#include "userio.h"
#include "file_io.h"

userInfo * curUsr;
userInfo * userList = NULL;             /*This points to the new user*/
FILE_READLN f_readln = file_readln;     /*Connect file_io with user */
IO_SHOWINFO showUsrMsg = io_showUsrMsg;

/*
 * @description:    This function is used to extract user data from a line
 */
STATUS_T user_extractLn(const char * line){
    authority auth;
    char id[ID_LEN] = {0};
    char pswd[PSWD_LEN] = {0};
    if(sscanf(line, "%d %s %s", &auth, id, pswd) == 3){
        userInfo* newUser = (userInfo*)malloc(sizeof(userInfo));
        if (newUser == NULL) {
            return MEMORY_ERROR;
        }
        newUser->auth = auth;
        strcpy(newUser->username, id);
        strcpy(newUser->password, pswd);
        newUser->next = userList;
        userList = newUser;
        return SUCCESS;
    }
    return USER_EXTRACT_ERROR;
}

STATUS_T user_add(authority auth, const char* id, const char* password){
    userInfo* newUser = (userInfo*)malloc(sizeof(userInfo));
    if (newUser == NULL) {
        return MEMORY_ERROR;
    }
    newUser->auth = auth;
    strcpy(newUser->username, id);
    strcpy(newUser->password, password);
    newUser->next = userList;
    userList = newUser;
    user_saveFile(USER_FILE);
    return SUCCESS;
}

/*
 * @description:    Find a certain user
 */
userInfo* user_find(const char* id) {
    userInfo* current = userList;
    while (current != NULL) {
        if (strcmp(current->username, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; /*No user*/
}

STATUS_T user_delete(const char* id) {
    userInfo* current = userList;
    userInfo* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->username, id) == 0) {   /*Match SUCCESS*/
            if (prev == NULL) {
                userList = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    return FAILURE;
}

/*
 * @description:    Read user data from file
 */
STATUS_T user_readFile(const char * fileName){
    int lineCnt = file_getLineNum(fileName);
    char fileLine[ID_LEN + PSWD_LEN + 4] = {0};
    for(int line = 1; line <= lineCnt; line++){
        f_readln(USER_FILE, line, fileLine);
        user_extractLn(fileLine);
    }
}

/*
 * @description:    Output all user's information in terminal
 */
void user_debugShowAll (void) {
    userInfo * up = userList;
    while(up != NULL){
        printf("%d %s %s\n", up->auth, up->username, up->password);
        up = up->next;
    }
}

void user_init (void) {
    user_readFile(USER_FILE);
//    memset(curUsr->username, 0, sizeof(curUsr->username));
}

STATUS_T user_match (const char * id, const char * pswd) {
    userInfo * up = userList;
    while(up != NULL) {
        if (strcmp(up->username, id) == 0) {        /*ID match success*/
            if(strcmp(up->password, pswd) == 0) {    /*PSWD match success*/
                return SUCCESS;
            }
            return PSWD_ERROR;
        }
        up = up->next;
    }
    return NO_USER_NAME;
}

authority user_getAuth (const char * id) {
    userInfo * up = userList;
    while(up != NULL){
        if (strcmp(up->username, id) == 0) {        /*ID match success*/
            return up->auth;
        }
        up = up->next;
    }
    return READ_ERROR;
}

STATUS_T user_ifExist (const char * id) {
    userInfo * up = userList;
    while(up != NULL) {
        if (strcmp(up->username, id) == 0) {        /*ID match success*/
            return USER_EXIST;
        }
        up = up->next;
    }
    return USER_AVAILABLE;
}

/*
 * @description:     This function is used to match the user-input account
 *                   and password with the existing account, and upon a successful match,
 *                   it stores the information in the 'currentUsr' variable
 */
STATUS_T user_login (char * id, char * pswd) {
    STATUS_T status;
    authority auth;
    status = user_match(id, pswd);
    if (status == SUCCESS) {
        curUsr = user_find(id);
    }
    return status;
}

STATUS_T user_logout (void) {
    curUsr = NULL;
    errword_clear();
}

STATUS_T user_saveFile (const char * fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return FILE_OPEN_ERROR;
    }

    userInfo * up = userList;
    while (up != NULL) {
        fprintf(file, "%d %s %s\n",
                up->auth, up->username, up->password);
        up = up->next;
    }
    fclose(file);
    return SUCCESS;
}
