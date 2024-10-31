//
// Created by Asaka on 2023/10/28.
//

#include "menu.h"

extern userInfo * curUsr;
menu_t * adminMenu;
menu_t * usrMenu;
menu_t * errWordMenu;
menu_t * startMenu;
menu_t * optionMenu;
menu_t * langMenu;

void menu_refresh(void) {
    system("clear");
}

void menu_init (void) {
    adminMenu = menu_create(7,
                            adminMenu_exitFun,
                            adminMenu_loadFun,
                            adminMenu_enterFun);
    usrMenu = menu_create(9,
                          usrMenu_exitFun,
                          usrMenu_loadFun,
                          usrMenu_enterFun);
    errWordMenu = menu_create(7,
                              errWordMenu_exitFun,
                              errWordMenu_loadFun,
                              errWordMenu_enterFun);
    startMenu = menu_create(3,
                            startMenu_exitFun,
                            startMenu_loadFun,
                            startMenu_enterFun);
    optionMenu = menu_create(1,
                             optionMenu_exitFun,
                             optionMenu_loadFun,
                             optionMenu_enterFun);
    langMenu = menu_create(2,
                           langMenu_exitFun,
                           langMenu_loadFun,
                           langMenu_enterFun);

//    menu_bindChildAndParentMenu(optionMenu, startMenu);
//    menu_bindChildAndParentMenu(langMenu, optionMenu);
    menu_bindChildAndParentMenu(errWordMenu, usrMenu);
}

menu_t * menu_create(int itemsNum,
                     pMenuExitFun_f pMenuExitFun,
                     pSelectionLoadFun_f pMenuLoadFun,
                     pMenuEnterFun_f pMenuEnterFun) {
    menu_t * newMenu = (menu_t*) malloc(sizeof(menu_t));
    newMenu->itemsNum = itemsNum;
    newMenu->pMenuExitFun = pMenuExitFun;
    newMenu->pMenuEnterFun = pMenuEnterFun;
    newMenu->pSelectionLoadFun = pMenuLoadFun;
    return newMenu;
}

void menu_bindLoadFun (menu_t * menu, pSelectionLoadFun_f fun) {
    menu->pSelectionLoadFun = fun;
}

void menu_bindChildAndParentMenu (menu_t * childMenu, menu_t * parentMenu) {
    childMenu->parentMenu = parentMenu;
    parentMenu->childMenu = childMenu;
}

/*
 * Administrator Function:
 * */

void adminMenu_loadFun (menuSize_t selection) {
    switch (selection) {
        case 1:
            menu_refresh();
            while (io_adminAddWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 2:
            menu_refresh();
            while (io_adminDeleteWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 3:
            menu_refresh();
            while (io_adminSearchWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 4:
            menu_refresh();
            while (io_adminEditWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 5:
            menu_refresh();
            while (io_adminShowWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 6:
            adminMenu->pMenuExitFun();
            break;
        case 7:
            user_logout();
            startMenu_enterFun();
        default:
            break;
    }
    adminMenu_enterFun();
}

void adminMenu_enterFun (void) {
    menu_refresh();
    adminMenu->selectedItem = (menuSize_t)io_adminMenu();
    adminMenu->pSelectionLoadFun(adminMenu->selectedItem);
}

void adminMenu_exitFun (void) {
    exit(0);
}

/*
 * User Function:
 * */

void usrMenu_loadFun (menuSize_t selection) {
    switch (selection) {
        case 1:
            menu_refresh();
            while (io_usrENG2CN() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 2:
            menu_refresh();
            while (io_usrCN2ENG() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 3:
            usrMenu->childMenu->pMenuEnterFun();
            break;
        case 4:
            menu_refresh();
            while (io_usrSearchWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 5:
            menu_refresh();
            while (io_usrShowWord() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 6:
            menu_refresh();
            while (io_practice_CN2ENG(WORD_FILE) == CONTINUE) {
                menu_refresh();
            }
            break;
        case 7:
            menu_refresh();
            while (io_practice_ENG2CN(WORD_FILE) == CONTINUE) {
                menu_refresh();
            }
            break;
        case 8:
            menu_refresh();
            io_usrImportFile();
            break;
        case 9:
            usrMenu->pMenuExitFun();
            break;
        case 10:
            user_logout();
            startMenu_enterFun();
        default:
            break;
    }
    usrMenu_enterFun();
}

void usrMenu_enterFun (void) {
    menu_refresh();
    usrMenu->selectedItem = (menuSize_t)io_userMenu();
    usrMenu->pSelectionLoadFun(usrMenu->selectedItem);
}

void usrMenu_exitFun (void) {
    exit(0);
}

/*
 *  Misspelled Word Functions:
 * */

void errWordMenu_loadFun (menuSize_t selection) {
    switch (selection) {
        case 1:
            menu_refresh();
            while (io_errwordDelete() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 2:
            menu_refresh();
            while (io_errwordSearch() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 3:
            menu_refresh();
            while (io_errwordEdit() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 4:
            menu_refresh();
            while (io_errwordAdd() == CONTINUE) {
                menu_refresh();
            }
            break;
        case 5:
            menu_refresh();
            while (io_errwordShow() != END);
            break;
        case 6:
            menu_refresh();
            while (io_errwordPractice_CN2ENG(ERRWORD_FILE) == CONTINUE) {
                menu_refresh();
            }
            break;
        case 7:
            menu_refresh();
            while (io_errwordPractice_ENG2CN(ERRWORD_FILE) == CONTINUE) {
                menu_refresh();
            }
            break;
        case 8:
            errWordMenu_exitFun();
            break;
        default:
            break;
    }
    errWordMenu_enterFun();
}

void errWordMenu_enterFun (void) {
    menu_refresh();
    errWordMenu->selectedItem = io_errWordMenu();
    errWordMenu->pSelectionLoadFun(errWordMenu->selectedItem);
}

void errWordMenu_exitFun (void) {
    errWordMenu->parentMenu->pMenuEnterFun();
}

/*
 * Start menu functions:
 * */

void startMenu_enterFun (void) {
    menu_refresh();
    startMenu->selectedItem = io_startMenu();
    startMenu->pSelectionLoadFun(startMenu->selectedItem);
}

void startMenu_exitFun (void) {
    exit(0);
}

void startMenu_loadFun (menuSize_t selection) {
    switch (selection) {
        case 1:
            menu_refresh();
            io_login();
            if (curUsr->auth == admin) {
                adminMenu->pMenuEnterFun();
            } else if (curUsr->auth == user) {
                usrMenu->pMenuEnterFun();
            }
            break;
        case 2:
            menu_refresh();
            io_register();
            startMenu->pMenuEnterFun();
            break;
        case 3:
            startMenu->pMenuExitFun();
            break;
        default:
            break;
    }
}

/*
 * Option menu functions:
 * */

void optionMenu_enterFun (void) {

}

void optionMenu_exitFun (void) {

}

void optionMenu_loadFun (menuSize_t selection) {

}

/*
 * LangMenu functions:
 * */

void langMenu_enterFun (void) {

}

void langMenu_exitFun (void) {

}

void langMenu_loadFun (menuSize_t selection) {

}
