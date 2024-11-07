cmake_minimum_required(VERSION 2.8...3.15)
project(edumper-usermode LANGUAGES C CXX)

add_executable(${PROJECT_NAME}) 
target_sources(${PROJECT_NAME} PRIVATE source/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC includes)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)