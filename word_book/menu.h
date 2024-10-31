//
// Created by Asaka on 2023/10/28.
//

#ifndef WORD_BOOK_MENU_H
#define WORD_BOOK_MENU_H
#include "userio.h"
#include "user.h"
#include "word.h"
#include "stdlib.h"
#include "stdint.h"

typedef uint8_t menuSize_t;

typedef void (*pMenuExitFun_f) (void);
typedef void (*pMenuEnterFun_f) (void);
typedef void (*pSelectionLoadFun_f) (menuSize_t selection);

typedef struct menu_t {
    menuSize_t menuMaxLang;
    menuSize_t itemsNum;
    menuSize_t selectedItem;
    struct menu_t * parentMenu;
    struct menu_t * childMenu;
    pMenuExitFun_f pMenuExitFun;              /*Menu exit function*/
    pMenuEnterFun_f pMenuEnterFun;            /*Menu enter function*/
    pSelectionLoadFun_f pSelectionLoadFun;    /*Selection load function*/
}menu_t;

menu_t * menu_create(int itemsNum,
                     pMenuExitFun_f pMenuExitFun,
                     pSelectionLoadFun_f pMenuLoadFun,
                     pMenuEnterFun_f pMenuEnterFun);
void menu_init (void);
void menu_refresh (void);
void menu_bindLoadFun (menu_t * menu, pSelectionLoadFun_f fun);
void menu_bindChildAndParentMenu (menu_t * childMenu, menu_t * parentMenu);

void adminMenu_enterFun (void);
void adminMenu_exitFun (void);
void adminMenu_loadFun (menuSize_t selection);

void usrMenu_enterFun (void);
void usrMenu_exitFun (void);
void usrMenu_loadFun (menuSize_t selection);

void errWordMenu_enterFun (void);
void errWordMenu_exitFun (void);
void errWordMenu_loadFun (menuSize_t selection);

void startMenu_enterFun (void);
void startMenu_exitFun (void);
void startMenu_loadFun (menuSize_t selection);

void optionMenu_enterFun (void);
void optionMenu_exitFun (void);
void optionMenu_loadFun (menuSize_t selection);

void langMenu_enterFun (void);
void langMenu_exitFun (void);
void langMenu_loadFun (menuSize_t selection);

#endif //WORD_BOOK_MENU_H
