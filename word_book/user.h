//
// Created by Asaka on 2023/10/26.
//
#ifndef _USER_H
#define _USER_H
#include "status.h"
#include "stdio.h"
#include "file_io.h"

#define ID_LEN      10
#define PSWD_LEN    15

typedef enum {
    admin = 0,
    user,
    READ_ERROR
}authority;

typedef struct userInfo{
    authority auth;
    char username[ID_LEN];
    char password[PSWD_LEN];
    struct userInfo* next;
}userInfo;          /* Save user information */


typedef void        (*IO_USRINFO)    (char *id, char *pswd);
typedef void        (*IO_SHOWINFO)   (STATUS_T);
typedef STATUS_T    (*FILE_READLN)   (const char *fileName, int lineNum, char *dst); /* Declare a function pointer */

void user_init (void);
void user_debugShowAll (void);
STATUS_T user_readFile (const char * fileName);
STATUS_T user_saveFile (const char * fileName);
userInfo * user_find (const char * id);
STATUS_T user_add (authority auth, const char* id, const char* password);
STATUS_T user_delete (const char * username);
STATUS_T user_extractLn (const char * line);
STATUS_T user_match (const char * id, const char * pswd);
STATUS_T user_login (char * id, char * password);
STATUS_T user_logout (void);
STATUS_T user_ifExist (const char * id);
authority user_getAuth (const char * id);


#endif //_USER_H
