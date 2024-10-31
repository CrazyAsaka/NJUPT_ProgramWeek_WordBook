#include <stdio.h>
#include "user.h"
#include "word.h"
#include "userio.h"
#include "menu.h"
#include "question.h"
#include "windows.h"
#include "time.h"
#include "Console_IO.h"

extern userInfo * curUsr;
extern menu_t * startMenu;
HANDLE hConsole;

int main() {
    user_init();
    word_init();
    menu_init();
//    errword_init();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    startMenu->pMenuEnterFun();
    return 0;
}
