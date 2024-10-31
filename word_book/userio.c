//
// Created by Asaka on 2023/10/27.
//
/*
 * @description:    This file is used to implement human-machine interaction functionality in terminal
 */
#include "userio.h"
#include "windows.h"
#include "time.h"
#include "Console_IO.h"
#include <inttypes.h>
#include <stdbool.h>

extern userInfo * curUsr;
extern errWordInfo * errWordList;
extern questSize_t totalQuestNum;
extern question_t * questList;

void io_readPassword(char password[]) {
    int index = 0;
    char ch;
    while ((ch = _getch()) != '\n') {
        if (ch == '\b') {
            if (index == 0) {
                printf("\a");
            } else {
                printf("\b");
            }
        } else {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
}

void io_login (void) {
    char id[ID_LEN];
    char pswd[PSWD_LEN];
    STATUS_T status = END;
    printf("Enter your username:\n");
    scanf("%s", id);
    inputFlush();
    printf("Enter your password:\n");
    io_readPassword(pswd);
//    scanf("%s", pswd);
    while ((status = user_login(id, pswd)) != SUCCESS) {
        io_showUsrMsg(status);
        Sleep(500);
        menu_refresh();
        printf("Enter your username:\n");
        scanf("%s", id);
        inputFlush();
        printf("Enter your password:\n");
        io_readPassword(pswd);
//        scanf("%s", pswd);
    }
    printf("Hello, %s\n",curUsr->username);
    errword_readFile(ERRWORD_FILE);
    Sleep(1000);
}

void io_register (void) {
    char id[ID_LEN];
    char pswd[PSWD_LEN];
    STATUS_T status = END;
    printf("Please enter the username:\n");
    scanf("%s", id);
    getchar();
    while (user_find(id) != NULL) {
        printf("The username already exists, please enter a different username.\n");
        textattr(GREEN);
        scanf("%s", id);
        normvideo();
    }
    printf("Please enter your password:\n");
//    scanf("%s", pswd);
    textattr(GREEN);
    io_readPassword(pswd);
    normvideo();
    status = user_add(user, id, pswd);
    io_showUsrMsg(status);
    Sleep(1000);
}

void io_showUsrMsg (STATUS_T status) {
    switch (status) {
        case NO_USER_NAME:
            textattr(RED);
            printf("NO such user!\n");
            normvideo();
            break;
        case PSWD_ERROR:
            textattr(RED);
            printf("Password is NOT correct!\n");
            normvideo();
            break;
        case USER_EXIST:
            textattr(RED);
            printf("User already exists!\n");
            normvideo();
            break;
        case FAILURE:
            textattr(RED);
            printf("ERROR!\n");
            normvideo();
            break;
        case NO_WORD:
            textattr(RED);
            printf("NO Such Word!\n");
            normvideo();
            break;
        case WORD_EXIST:
            textattr(RED);
            printf("Word already exists!\n");
            normvideo();
            break;
        case SUCCESS:
            textattr(GREEN);
            printf("Success!\n");
            normvideo();
        default:
            break;
    }
}

menuSize_t io_adminMenu (void) {
    menuSize_t choice;
    printf("Admin Menu:\n");
    printf("1. Add word\n");
    printf("2. Delete word\n");
    printf("3. Search word\n");
    printf("4. Edit word\n");
    printf("5. Show word\n");
    printf("6. Exit\n");
    printf("7. Log out\n");
    printf("Please enter your choice:\n");
    textattr(GREEN);
    bool validInput = false;
    while (!validInput) {
        if (scanf("%" SCNu8, &choice) == 1 && choice >= 1 && choice <= 7) {
            validInput = true;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            logoutMsg("Please enter again!\n", RED);
        }
    }
    normvideo();
    return choice;
}

menuSize_t io_userMenu (void) {
    menuSize_t choice;
    printf("User Menu:\n");
    printf("1. English to Chinese\n");
    printf("2. Chinese to English\n");
    printf("3. Dictionary of misspelled words\n");
    printf("4. Search word\n");
    printf("5. Show word\n");
    printf("6. Practice--CN to ENG\n");
    printf("7. Practice--ENG to CN\n");
    printf("8. Import another word file\n");
    printf("9. Exit\n");
    printf("10. Log out\n");
    printf("Please enter your choice:\n");
    textattr(GREEN);
    bool validInput = false;
    while (!validInput) {
        if (scanf("%" SCNu8, &choice) == 1 && choice >= 1 && choice <= 10) {
            validInput = true;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            logoutMsg("Please enter again!\n", RED);
        }
    }
    normvideo();
    return choice;
}

menuSize_t io_errWordMenu (void) {
    menuSize_t choice;
    printf("Misspelled Word Menu:\n");
    printf("1. Delete word\n");
    printf("2. Search word\n");
    printf("3. Edit word\n");
    printf("4. Add word\n");
    printf("5. Show word\n");
    printf("6. Practice--CN to ENG\n");
    printf("7. Practice--ENG to CN\n");
    printf("8. Return\n");
    printf("Please enter your choice:\n");
    textattr(GREEN);
    bool validInput = false;
    while (!validInput) {
        if (scanf("%" SCNu8, &choice) == 1 && choice >= 1 && choice <= 8) {
            validInput = true;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            logoutMsg("Please enter again!\n", RED);
        }
    }
    normvideo();
    return choice;
}

menuSize_t io_startMenu (void) {
    menuSize_t choice;
    printf("Welcome to our system!\n");
    printf("1. Log in\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Please enter your choice:\n");
//    inputFlush();
    textattr(GREEN);
    bool validInput = false;
    while (!validInput) {
        if (scanf("%" SCNu8, &choice) == 1 && choice >= 1 && choice <= 3) {
            validInput = true;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            logoutMsg("Please enter again!\n", RED);
        }
    }
    normvideo();
    return choice;
}

STATUS_T io_adminAddWord (void) {
    char word[WORD_LEN];
    char part[PART_LEN];
    char meaning[MEAN_LEN];
    char sign = '0';
    STATUS_T status;
    printf("Please input the word in the following format:\n");
    printf("<word> <part> <meaning>, like: file n. 文件\n");
    textattr(GREEN);
    scanf("%s %s %s", word, part, meaning);
    normvideo();
    getchar();
    status = word_add(word, part, meaning);
    if (status == SUCCESS) {
        printf("'%s' is successfully added!\n",word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        while (sign != 'Y' && sign != 'N') {
            logoutMsg("Please enter again!\n", RED);
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        io_showUsrMsg(status);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
        return status;
    }
    return FAILURE;
}

STATUS_T io_usrImportFile (void) {
    char fileName[ID_LEN];
    STATUS_T status;
    printf("Please enter the filename:\n");
    textattr(GREEN);
    scanf("%s", fileName);
    normvideo();
    if (access(fileName, F_OK) == 0) {
//        printf("文件 %s 存在\n", fileName);
        word_clear();
        word_readFile(fileName);
        printf("Import success!\n");
        Sleep(1000);
    } else {
//        printf("文件 %s 不存在\n", fileName);
        logoutMsg("File does NOT EXIST!\n", RED);
        Sleep(1000);
    }
}

STATUS_T io_adminDeleteWord (void) {
    char word[WORD_LEN];
    char sign = '0';
    STATUS_T status;
    printf("Please enter the word you want to delete:\n");
    textattr(GREEN);
    scanf("%s", word);
    getchar();
    normvideo();
    status = word_delete(word);
    if (status == SUCCESS) {
        printf("'%s' is successfully deleted!\n", word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        while (sign != 'Y' && sign != 'N') {
            logoutMsg("Please enter again!\n", RED);
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        io_showUsrMsg(status);

        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        } else {
            return status;
        }
    }
    return FAILURE;
}

STATUS_T io_adminSearchWord (void) {
    char word[WORD_LEN];
    char sign = '0';
    wordInfo * wp;
    STATUS_T status;
    printf("Please enter the word you want to search:\n");
    textattr(GREEN);
    scanf("%s", word);
    getchar();
    normvideo();
    wp = word_find(word);
    if (wp != NULL) {
        printf("%s\t\t%s\t%s\n", wp->word, wp->part, wp->meaning);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        textattr(RED);
        printf("NO Such Word!\n");
        normvideo();
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        normvideo();
        while (sign != 'Y' && sign != 'N') {
            logoutMsg("Please enter again!\n", RED);
            sign = getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
    return FAILURE;
}

STATUS_T io_adminEditWord (void) {
    char word[WORD_LEN];
    char part[PART_LEN];
    char meaning[MEAN_LEN];
    char sign = '0';
    wordInfo * wp;
    STATUS_T status;
    printf("Please enter the word you want to edit:\n");
    textattr(GREEN);
    scanf("%s", word);
    normvideo();
    printf("Enter the new part of speech:\n");
    textattr(GREEN);
    scanf("%s", part);
    normvideo();
    printf("Enter the new meaning:\n");
    textattr(GREEN);
    scanf("%s", meaning);
    normvideo();
    getchar();
    status = word_edit(word, part, meaning);
    if (status == SUCCESS) {
        printf("'%s' is edited successfully!\n", word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        sign = getchar();
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        io_showUsrMsg(status);
        printf("Continue? Y/N\n");
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
    return FAILURE;
}

STATUS_T io_adminShowWord (void) {
    char sign = '0';
    word_showAll();
    printf("Press 'E' to exit\n");
    textattr(GREEN);
    static int i;
    while (sign != 'E') {
        i++;
        if (i >= 3) {
            logoutMsg("Please enter again!\n", RED);
        }
        sign =  getchar();
    }
    normvideo();
    return END;
}

/*
 * USER INPUT/OUTPUT
 * */

STATUS_T io_usrENG2CN (void) {
    STATUS_T status;
    char sign = '0';
    char word[WORD_LEN];
    char meaning[MEAN_LEN];
    printf("Please enter the word:\n");
    scanf("%s", word);
    getchar();
    status = word_eng2cn(word, meaning);
    if (status == SUCCESS) {
        printf("The meaning is : %s\n", meaning);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        io_showUsrMsg(status);
        printf("Continue? Y/N:\n");
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
        return status;
    }
    return FAILURE;
}
STATUS_T io_usrCN2ENG (void) {
    STATUS_T status;
    char sign = '0';
    char word[WORD_LEN];
    char meaning[MEAN_LEN];
    printf("Please enter the meaning:\n");
    scanf("%s", meaning);
//    printf("%s\n", meaning);
    status = word_cn2eng(meaning, word);
    if (status == SUCCESS) {
        printf("The word is : %s\n", word);
        printf("Continue? Y/N:\n");
        inputFlush();
        while (sign != 'Y' && sign != 'N') {
            logoutMsg("Please enter again!\n", RED);
            sign =  getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        io_showUsrMsg(status);

        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
        return status;
    }
    return FAILURE;
}

STATUS_T io_usrSearchWord (void) {
    char word[WORD_LEN];
    char sign = '0';
    wordInfo * wp;
    STATUS_T status;
    printf("Please enter the word you want to search:\n");
    scanf("%s", word);
    inputFlush();
    wp = word_find(word);
    if (wp != NULL) {
        printf("%s\t\t%s\t%s\n", wp->word, wp->part, wp->meaning);
        printf("Continue? Y/N:\n");
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        printf("NO Such Word!\n");
        printf("Continue? Y/N:\n");
        while (sign != 'Y' && sign != 'N') {
            logoutMsg("Please enter again!\n", RED);
            sign = getchar();
        }
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
        return NO_WORD;
    }
    return FAILURE;
}

STATUS_T io_usrShowWord (void) {
    char sign = '0';
    word_showAll();
    printf("Press 'E' to exit\n");
    textattr(GREEN);
    static int i;
    while (sign != 'E') {
        i++;
        if (i >= 3) {
            logoutMsg("Please enter again!\n", RED);
        }
        sign =  getchar();
    }
    normvideo();
    return END;
}

STATUS_T io_errwordPractice_CN2ENG (const char * file) {
    char key, sign = '0';
    int nums;
    float marks = 0;
    STATUS_T status;
    quest_srand();
    printf("Please enter the numbers of exercises:\n");
    textattr(GREEN);
    scanf("%d", &nums);
    normvideo();
    status = quest_create_errword(file, nums);
    question_t * qp = questList;
    if (status == SUCCESS) {
        while (qp != NULL) {
            printf("%d. What is the English of %s:\n", qp->questSeqNum, qp->questWord->meaning);
            printf("A: %s  B: %s  C: %s  D: %s\n",
                   qp->selectionWords[0]->word,
                   qp->selectionWords[1]->word,
                   qp->selectionWords[2]->word,
                   qp->selectionWords[3]->word);
            textattr(GREEN);
            getchar();
            key = getchar();
            while (key != 'A' && key != 'B' && key != 'C' && key != 'D') {
                key = getchar();
            }
            normvideo();
            qp->selectedAnsIndex = key - 'A';
            if (qp->corrAnsIndex == qp->selectedAnsIndex) {
                marks += 100.0/totalQuestNum;
                printf("Correct!\n");
            } else {
                printf("Wrong!\n");
            }
            qp = qp->next;
        }
        questList = NULL;
        printf("Your marks: %.2f", marks);
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        printf("The number of exercises set has exceeded the total word count!\n");
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
}

STATUS_T io_errwordPractice_ENG2CN (const char * file) {
    char key, sign = '0';
    float marks = 0;
    int nums;
    quest_srand();
    printf("Please enter the numbers of exercises:\n");
    textattr(GREEN);
    scanf("%d", &nums);
    normvideo();
    STATUS_T status;
    status = quest_create_errword(file, nums);
    question_t * qp = questList;
    if (status == SUCCESS) {
        while (qp != NULL) {
            printf("%d. What is the Meaning of %s:\n", qp->questSeqNum, qp->questWord->word);
            printf("A: %s  B: %s  C: %s  D: %s\n",
                   qp->selectionWords[0]->meaning,
                   qp->selectionWords[1]->meaning,
                   qp->selectionWords[2]->meaning,
                   qp->selectionWords[3]->meaning);
            textattr(GREEN);
            getchar();
            key = getchar();
            while (key != 'A' && key != 'B' && key != 'C' && key != 'D') {
                key = getchar();
            }
            normvideo();
            qp->selectedAnsIndex = key - 'A';
            if (qp->corrAnsIndex == qp->selectedAnsIndex) {
                marks += 100.0/totalQuestNum;
                printf("Correct!\n");
            } else {
                printf("Wrong!\n");
            }
            qp = qp->next;
        }
        questList = NULL;
        printf("Your marks: %.2f", marks);
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        printf("The number of exercises set has exceeded the total word count!\n");
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign = getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
}

STATUS_T io_practice_CN2ENG (const char * file) {
    char key, sign = '0';
    int nums;
    float marks = 0;
    STATUS_T status;
    quest_srand();
    printf("Please enter the numbers of exercises:\n");
    textattr(GREEN);
    scanf("%d", &nums);
    normvideo();
    status = quest_create(file, nums);
    question_t * qp = questList;
    if (status == SUCCESS) {
        while (qp != NULL) {
            printf("%d. What is the English of %s:\n", qp->questSeqNum, qp->questWord->meaning);
            printf("A: %s  B: %s  C: %s  D: %s\n",
                   qp->selectionWords[0]->word,
                   qp->selectionWords[1]->word,
                   qp->selectionWords[2]->word,
                   qp->selectionWords[3]->word);
            textattr(GREEN);
            getchar();
            key = getchar();
            while (key != 'A' && key != 'B' && key != 'C' && key != 'D') {
                key = getchar();
            }
            normvideo();
            qp->selectedAnsIndex = key - 'A';
            if (qp->corrAnsIndex == qp->selectedAnsIndex) {
                marks += 100.0/totalQuestNum;
                printf("Correct!\n");
            } else {
                errword_add(qp->questWord->word);
                printf("Wrong!\n");
            }
            qp = qp->next;
        }
        questList = NULL;
        printf("Your marks: %.2f", marks);
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        printf("The number of exercises set has exceeded the total word count!\n");
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
}

STATUS_T io_practice_ENG2CN (const char * file) {
    char key, sign = '0';
    float marks = 0;
    int nums;
    quest_srand();
    printf("Please enter the numbers of exercises:\n");
    textattr(GREEN);
    scanf("%d", &nums);
    normvideo();
    STATUS_T status;
    status = quest_create(file, nums);
    question_t * qp = questList;
    if (status == SUCCESS) {
        while (qp != NULL) {
            printf("%d. What is the Meaning of %s:\n", qp->questSeqNum, qp->questWord->word);
            printf("A: %s  B: %s  C: %s  D: %s\n",
                   qp->selectionWords[0]->meaning,
                   qp->selectionWords[1]->meaning,
                   qp->selectionWords[2]->meaning,
                   qp->selectionWords[3]->meaning);
            textattr(GREEN);
            getchar();
            key = getchar();
            while (key != 'A' && key != 'B' && key != 'C' && key != 'D') {
                key = getchar();
            }
            normvideo();
            qp->selectedAnsIndex = key - 'A';
            if (qp->corrAnsIndex == qp->selectedAnsIndex) {
                marks += 100.0/totalQuestNum;
                printf("Correct!\n");
            } else {
                errword_add(qp->questWord->word);
                printf("Wrong!\n");
            }
            qp = qp->next;
        }
        questList = NULL;
        printf("Your marks: %.2f", marks);
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        printf("The number of exercises set has exceeded the total word count!\n");
        printf("\nContinue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
}

/*
 * ERROR WORD FUNCTION IO
 * */

STATUS_T io_errwordDelete (void) {
    char sign = '0';
    char word[WORD_LEN];
    STATUS_T status;
    printf("Please enter the word you want to delete:\n");
    scanf("%s", word);
    getchar();
    status = errword_delete(word);
    if (status == SUCCESS) {
        printf("'%s' is successfully deleted!\n", word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        io_showUsrMsg(status);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
}

STATUS_T io_errwordSearch (void) {
    char word[WORD_LEN];
    char sign = '0';
    errWordInfo * wp;
    STATUS_T status;
    printf("Please enter the word you want to search:\n");
    scanf("%s", word);
    getchar();
    wp = errword_find(word);
    if (wp != NULL) {
        printf("%s\t\t%s\t%s\n", wp->word, wp->part, wp->meaning);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        printf("NO Such Word!\n");

        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 3) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    }
    return FAILURE;
}

STATUS_T io_errwordEdit (void) {
    char word[WORD_LEN];
    char part[PART_LEN];
    char meaning[MEAN_LEN];
    char sign = '0';
    errWordInfo * wp;
    STATUS_T status;
    printf("Please enter the word you want to edit:\n");
    scanf("%s", word);
    printf("Enter the new part of speech:\n");
    scanf("%s", part);
    printf("Enter the new meaning:\n");
    scanf("%s", meaning);
    getchar();
    status = errword_edit(word, part, meaning);
    if (status == SUCCESS) {
        printf("'%s' is edited successfully!\n", word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
    } else {
        io_showUsrMsg(status);

        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
        return status;
    }
    return FAILURE;
}

STATUS_T io_errwordAdd (void) {
    char word[WORD_LEN];
    char sign = '0';
    STATUS_T status;
    printf("Please input the word:\n");
    scanf("%s", word);
    getchar();
    status = errword_add(word);
    if (status == SUCCESS) {
        printf("'%s' is successfully added!\n",word);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N'){
            return END;
        }
    } else {
        io_showUsrMsg(status);
        printf("Continue? Y/N:\n");
        textattr(GREEN);
        static int i;
        while (sign != 'Y' && sign != 'N') {
            i++;
            if (i >= 2) {
                logoutMsg("Please enter again!\n", RED);
            }
            sign =  getchar();
        }
        normvideo();
        if (sign == 'Y') {
            return CONTINUE;
        } else if (sign == 'N') {
            return END;
        }
        return status;
    }
    return FAILURE;
}

STATUS_T io_errwordShow (void) {
    errWordInfo * wp = errWordList;
    char sign = '0';
    while(wp != NULL){
        printf("%s\t\t%s\t%s\n",wp->word, wp->part, wp->meaning);
        wp = wp->next;
    }
    printf("Press 'E' to exit\n");
    textattr(GREEN);
    static int i;
    while (sign != 'E') {
        i++;
        if (i >= 3) {
            logoutMsg("Please enter again!\n", RED);
        }
        sign =  getchar();
    }
    normvideo();
    return END;
}

/*
 * START MENU FUNCTION IO
 * */


