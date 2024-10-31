//
// Created by Asaka on 2023/10/31.
//

#include "question.h"

extern wordInfo * wordList;
question_t * questList = NULL;
questSize_t totalQuestNum;
extern wordSize_t wordTotalNum;
extern wordSize_t errWordTotalNum;
void quest_srand (void) {
    srand((unsigned)time(NULL));
}

/*
 * @description:    Import source file and create random questions/answers
 */
STATUS_T quest_create (const char * sourceFile, questSize_t _questNum) {
    if (_questNum > wordTotalNum) {
        return QUEST_NUM_OVERFLOW;
    }
    quest_srand();
    totalQuestNum = _questNum;
    wordInfo * wordSelect[SELECTION_NUMS-1];
    word_readFile(sourceFile);
    for (int index=1; index<=totalQuestNum; index++) {
        question_t * newQuest = (question_t*)malloc(sizeof(question_t));
        newQuest->questSeqNum = totalQuestNum - index + 1;

        newQuest->questWord = word_getWord(rand() % wordTotalNum);
        while (newQuest->questWord->sign == quested) {
            newQuest->questWord = word_getWord(rand() % wordTotalNum);
        }
        newQuest->questWord->sign = quested;

        wordSelect[0] = word_getWord(rand() % wordTotalNum);
        wordSelect[1] = word_getWord(rand() % wordTotalNum);
        wordSelect[2] = word_getWord(rand() % wordTotalNum);
        while (strcmp(wordSelect[0]->word,newQuest->questWord->word) == 0) {
            wordSelect[0] = word_getWord(rand() % wordTotalNum);
        }
        while (strcmp(wordSelect[1]->word,newQuest->questWord->word) == 0 ||
                strcmp(wordSelect[1]->word,wordSelect[0]->word) == 0) {
            wordSelect[1] = word_getWord(rand() % wordTotalNum);
        }
        while (strcmp(wordSelect[2]->word,newQuest->questWord->word) == 0 ||
                strcmp(wordSelect[2]->word,wordSelect[0]->word) == 0 ||
                strcmp(wordSelect[2]->word,wordSelect[1]->word) == 0) {
            wordSelect[2] = word_getWord(rand() % wordTotalNum);
        }

        newQuest->corrAnsIndex = rand() % 4;
        switch (newQuest->corrAnsIndex) {
            case 0:
                newQuest->selectionWords[0] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 1:
                newQuest->selectionWords[1] = newQuest->questWord;
                newQuest->selectionWords[0] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 2:
                newQuest->selectionWords[2] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[0] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 3:
                newQuest->selectionWords[3] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[0] = wordSelect[2];
                break;
        }
        newQuest->next = questList;
        questList = newQuest;
    }
    return SUCCESS;
}

STATUS_T quest_create_errword(const char * sourceFile, questSize_t _questNum) {
    if (_questNum > errWordTotalNum) {
        return QUEST_NUM_OVERFLOW;
    }
    quest_srand();
    totalQuestNum = _questNum;
    errWordInfo * wordSelect[SELECTION_NUMS-1];
    errword_readFile(sourceFile);
    for (int index=1; index<=totalQuestNum; index++) {
        question_t * newQuest = (question_t*)malloc(sizeof(question_t));
        newQuest->questSeqNum = totalQuestNum - index + 1;

        newQuest->questWord = errword_getWord(rand() % errWordTotalNum);
        while (newQuest->questWord->sign == quested) {
            newQuest->questWord = errword_getWord(rand() % errWordTotalNum);
        }
        newQuest->questWord->sign = quested;

        wordSelect[0] = errword_getWord(rand() % errWordTotalNum);
        wordSelect[1] = errword_getWord(rand() % errWordTotalNum);
        wordSelect[2] = errword_getWord(rand() % errWordTotalNum);
        while (strcmp(wordSelect[0]->word,newQuest->questWord->word) == 0) {
            wordSelect[0] = errword_getWord(rand() % errWordTotalNum);
        }
        while (strcmp(wordSelect[1]->word,newQuest->questWord->word) == 0 ||
               strcmp(wordSelect[1]->word,wordSelect[0]->word) == 0) {
            wordSelect[1] = errword_getWord(rand() % errWordTotalNum);
        }
        while (strcmp(wordSelect[2]->word,newQuest->questWord->word) == 0 ||
               strcmp(wordSelect[2]->word,wordSelect[0]->word) == 0 ||
               strcmp(wordSelect[2]->word,wordSelect[1]->word) == 0) {
            wordSelect[2] = errword_getWord(rand() % errWordTotalNum);
        }

        newQuest->corrAnsIndex = rand() % 4;
        switch (newQuest->corrAnsIndex) {
            case 0:
                newQuest->selectionWords[0] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 1:
                newQuest->selectionWords[1] = newQuest->questWord;
                newQuest->selectionWords[0] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 2:
                newQuest->selectionWords[2] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[0] = wordSelect[1];
                newQuest->selectionWords[3] = wordSelect[2];
                break;
            case 3:
                newQuest->selectionWords[3] = newQuest->questWord;
                newQuest->selectionWords[1] = wordSelect[0];
                newQuest->selectionWords[2] = wordSelect[1];
                newQuest->selectionWords[0] = wordSelect[2];
                break;
        }
        newQuest->next = questList;
        questList = newQuest;
    }
    return SUCCESS;
}

void quest_show (void) {
    question_t * qp = questList;
    int i=1;
    while (qp != NULL) {
        printf("%d. %s\nA:%s B:%s C:%s D:%s\n",
               i++,
               qp->questWord->word,
               qp->selectionWords[0]->word,
               qp->selectionWords[1]->word,
               qp->selectionWords[2]->word,
               qp->selectionWords[3]->word);
        qp = qp->next;
    }
}
