cmake_minimum_required(VERSION 3.29)
project(gameprojecty)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(gameprojecty
    main.cpp
    uibuttons.cpp
    uibuttons.h
    pdcurses.a,
        panel.a
)
