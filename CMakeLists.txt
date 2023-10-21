cmake_minimum_required(VERSION 3.26)
project(McCoyImageProcessor C)

set(CMAKE_C_STANDARD 99)

add_executable(McCoyImageProcessor main.c
        Image.h
        BMPHandler.h
        BMPHandler.c
        Image.c)
