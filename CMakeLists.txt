cmake_minimum_required(VERSION 3.30)
project(SudokuProject C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(SudokuProject
    main.c
        src/sudoku.c
        src/sudoku.h
        src/ui.c
        src/ui.h
        src/save.c
        src/save.h
        src/anneal.c
        src/anneal.h
)
