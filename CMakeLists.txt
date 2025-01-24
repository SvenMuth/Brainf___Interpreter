cmake_minimum_required(VERSION 3.30)
project(BrainfuckInterpreter C)

set(CMAKE_C_STANDARD 23)

add_executable(BrainfuckInterpreter main.c
        main.h
)
