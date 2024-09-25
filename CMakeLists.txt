cmake_minimum_required(VERSION 3.20)
project(crypto-practice)

include_directories(".")
set(TASK_1_SRC task1.cpp misc.cpp YAFC.cpp paddings.cpp)

add_executable(task1 ${TASK_1_SRC})