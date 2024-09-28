cmake_minimum_required(VERSION 3.20)
project(crypto-practice)

include_directories(".")
set(TASK_1_SRC task1.cpp misc.cpp YAFC.cpp paddings.cpp)

include(FetchContent) # If not included already

FetchContent_Declare(sciplot_content
  GIT_REPOSITORY https://github.com/sciplot/sciplot.git
  GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
  FetchContent_Populate(sciplot_content)
endif()

include_directories(${sciplot_content_SOURCE_DIR})

add_executable(task1 ${TASK_1_SRC})
add_executable(task2 task2.cpp)