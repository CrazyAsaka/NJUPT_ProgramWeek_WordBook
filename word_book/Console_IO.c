//
// Created by Asaka on 2023/11/5.
//

#include "Console_IO.h"

static text_info __text_info = {
        1, 1,
        LIGHTGRAY + (BLACK << 4),
        LIGHTGRAY + (BLACK << 4),
        80, 25
};
static int __CONIO_TOP = 0;
static int __CONIO_LEFT = 0;

static void __fill_text_info (void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    __CONIO_LEFT = info.srWindow.Left;
    __CONIO_TOP = info.srWindow.Top;
    __text_info.curx = info.dwCursorPosition.X - __CONIO_LEFT + 1;
    __text_info.cury = info.dwCursorPosition.Y - __CONIO_TOP  + 1;
    __text_info.attribute = info.wAttributes;
    __text_info.screenwidth  = info.srWindow.Right - info.srWindow.Left + 1;
    __text_info.screenheight = info.srWindow.Bottom - info.srWindow.Top + 1;
}

int _getch (void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void inputFlush (void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void textcolor (int color)
{
    __FOREGROUND = color;
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             color + (__BACKGROUND << 4));
}

void puttext (int left, int top, int right, int bottom, struct char_info * buf)
{
    int i;
    SMALL_RECT r= { (short)(__CONIO_LEFT + left - 1), (short)(__CONIO_TOP + top - 1), (short)(__CONIO_LEFT + right - 1), (short)(__CONIO_TOP + bottom - 1) };
    CHAR_INFO* buffer;
    COORD size;
    COORD coord = { 0,0 };

    __fill_text_info();
    size.X = right - left + 1;
    size.Y = bottom - top + 1;
    buffer = (CHAR_INFO*)malloc (size.X * size.Y * sizeof(CHAR_INFO));

    for (i = 0; i < size.X * size.Y; i++)
    {
#ifdef UNICODE
        buffer[i].Char.UnicodeChar = buf[i].letter;
#else
        buffer[i].Char.AsciiChar = buf[i].letter;
#endif
        buffer[i].Attributes = buf[i].attr;
    }

    WriteConsoleOutput (GetStdHandle (STD_OUTPUT_HANDLE), buffer, size, coord, &r);
    free (buffer);
}

void movetext (int left, int top, int right, int bottom, int destleft, int desttop)
{
    struct char_info * buffer;

    buffer = (char_info*)malloc ((right - left + 1) * (bottom - top + 1) * sizeof(struct char_info));  // -----
    gettext (left, top, right, bottom, buffer);
    puttext (destleft, desttop, destleft + right - left, desttop + bottom - top, buffer);
    free(buffer);
}

void _conio_gettext (int left, int top, int right, int bottom,
                     struct char_info * buf)
{
    int i;
    SMALL_RECT r= { (short)(__CONIO_LEFT + left - 1), (short)(__CONIO_TOP + top - 1), (short)(__CONIO_LEFT + right - 1), (short)(__CONIO_TOP + bottom - 1) };
    CHAR_INFO* buffer;
    COORD size;
    COORD coord = {0,0};

    __fill_text_info();
    size.X = right - left + 1;
    size.Y = bottom - top + 1;
    buffer = (CHAR_INFO*)malloc (size.X * size.Y * sizeof(CHAR_INFO));

    ReadConsoleOutput (GetStdHandle (STD_OUTPUT_HANDLE),
                       (PCHAR_INFO) buffer, size, coord, &r);

    for (i = 0; i < size.X * size.Y; i++)
    {
#ifdef UNICODE
        buf[i].letter = buffer[i].Char.UnicodeChar;
#else
        buf[i].letter = buffer[i].Char.AsciiChar;
#endif
        buf[i].attr = buffer[i].Attributes;
    }
    free (buffer);
}

void gotoxy(int x, int y)
{
    COORD c;

    c.X = __CONIO_LEFT + x - 1;
    c.Y = __CONIO_TOP  + y - 1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void _setcursortype (int type)
{
    CONSOLE_CURSOR_INFO Info;

    if (type == 0) {
        Info.bVisible = FALSE;
    } else {
        Info.dwSize = type;
        Info.bVisible = TRUE;
    }
    SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE),
                          &Info);
}

void textattr (int _attr)
{
    __FOREGROUND = _attr & 0xF;
    __BACKGROUND = _attr >> 4;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}

void normvideo (void)
{
    textattr (__text_info.normattr);
}

void textbackground (int color)
{
    __BACKGROUND = color;
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             __FOREGROUND + (color << 4));
}

int wherex (void)
{
    __fill_text_info();
    return __text_info.curx;
}


int wherey (void)
{
    __fill_text_info();
    return __text_info.cury;
}

void highvideo (void)
{
    if (__FOREGROUND < DARKGRAY) textcolor(__FOREGROUND + 8);
}

void lowvideo (void)
{
    if (__FOREGROUND > LIGHTGRAY) textcolor(__FOREGROUND - 8);
}

void delay (int ms)
{
    Sleep(ms);
}

void switchbackground (int color)
{
    struct char_info* buffer;
    int i;

    buffer = (char_info*)malloc(__text_info.screenwidth * __text_info.screenheight *
                                sizeof(struct char_info));
    _conio_gettext(1, 1, __text_info.screenwidth, __text_info.screenheight,
                   buffer);
    for (i = 0; i < __text_info.screenwidth * __text_info.screenheight; i++) {
        unsigned short attr = buffer[i].attr & 0xF;
        buffer[i].attr = (color << 4) | attr;
    }
    puttext(1, 1, __text_info.screenwidth, __text_info.screenheight, buffer);
    free(buffer);
}

void flashbackground (int color, int ms) {
    struct char_info* buffer;

    buffer = (char_info*)malloc(__text_info.screenwidth * __text_info.screenheight *
                                sizeof(struct char_info));
    _conio_gettext(1, 1, __text_info.screenwidth, __text_info.screenheight,
                   buffer);
    switchbackground(color);
    delay(ms);
    puttext(1, 1, __text_info.screenwidth, __text_info.screenheight, buffer);
    free(buffer);
}

void logoutMsg (const char * msg, COLORS color) {
    textattr(color);
    printf("%s", msg);
    normvideo();
}
