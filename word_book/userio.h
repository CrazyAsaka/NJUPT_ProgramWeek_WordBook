//
// Created by Asaka on 2023/10/27.
//

#ifndef WORD_BOOK_USERIO_H
#define WORD_BOOK_USERIO_H
#include "status.h"
#include "stdio.h"
#include "user.h"
#include "word.h"
#include "windows.h"
#include "question.h"
#include "menu.h"

typedef uint8_t menuSize_t;

void io_showUsrMsg (STATUS_T);
void io_readPassword(char password[]);
menuSize_t io_adminMenu (void);
menuSize_t io_userMenu (void);
menuSize_t io_errWordMenu (void);
menuSize_t io_startMenu (void);

STATUS_T io_adminAddWord (void);
STATUS_T io_adminDeleteWord (void);
STATUS_T io_adminSearchWord (void);
STATUS_T io_adminEditWord (void);
STATUS_T io_adminShowWord (void);

STATUS_T io_usrImportFile (void);
STATUS_T io_usrENG2CN (void);
STATUS_T io_usrCN2ENG (void);
STATUS_T io_usrSearchWord (void);
STATUS_T io_usrShowWord (void);
STATUS_T io_practice_CN2ENG (const char * file);
STATUS_T io_practice_ENG2CN (const char * file);

STATUS_T io_errwordDelete (void);
STATUS_T io_errwordSearch (void);
STATUS_T io_errwordEdit (void);
STATUS_T io_errwordAdd (void);
STATUS_T io_errwordShow (void);
STATUS_T io_errwordPractice_ENG2CN (const char * file);
STATUS_T io_errwordPractice_CN2ENG (const char * file);

void io_login (void);
void io_register (void);

#endif //WORD_BOOK_USERIO_H
