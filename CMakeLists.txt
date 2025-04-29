cmake_minimum_required(VERSION 3.30)
project(SudokuProject C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SudokuProject
    main.c
    sudoku.c
    sudoku.h
    ui.c
    ui.h)
