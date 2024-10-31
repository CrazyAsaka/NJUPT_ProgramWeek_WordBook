cmake_minimum_required(VERSION 3.26)
project(word_book C)

set(CMAKE_C_STANDARD 99)

add_executable(word_book main.c
        user.c
        user.h
        status.h
        file_io.c
        file_io.h
        userio.c
        userio.h
        word.c
        word.h
        menu.c
        menu.h
        question.c
        question.h
        Console_IO.c
        Console_IO.h
)
