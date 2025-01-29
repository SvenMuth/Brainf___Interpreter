cmake_minimum_required(VERSION 3.30)
project(BrainfuckInterpreter C)

set(CMAKE_C_STANDARD 23)

OPTION(DEBUG "debug mode" OFF)
OPTION(NO_LOG "Write output directly to console" OFF)

IF (DEBUG AND NO_LOG)
    message( FATAL_ERROR "It is only allowed to set one option!")
ENDIF(DEBUG AND NO_LOG)

IF(DEBUG)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF(DEBUG)

IF(NO_LOG)
    ADD_DEFINITIONS(-DNO_LOG)
ENDIF(NO_LOG)

add_executable(BrainfuckInterpreter main.c
        interpreter.c
        interpreter.h
        debugger.c
        debugger.h
        colors.h
)
