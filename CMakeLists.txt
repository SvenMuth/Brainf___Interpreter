cmake_minimum_required(VERSION 3.30)
project(BrainfuckInterpreter C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-DDEBUG")
#set(CMAKE_C_FLAGS "-DNO_LOG")

add_executable(BrainfuckInterpreter main.c
        list.c
        list.h
        interpreter.c
        interpreter.h
        debugger.c
        debugger.h
        colors.h
)
