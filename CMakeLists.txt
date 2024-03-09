cmake_minimum_required(VERSION 3.27)
project(FolderAnlysis)

set(CMAKE_CXX_STANDARD 17)

add_executable(FolderAnlysis main.cpp
        Folder.h
        Folder.cpp
        Tree.h
        Tree.cpp
        Detection.h
        Detection.cpp
)
