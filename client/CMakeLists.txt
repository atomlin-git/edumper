cmake_minimum_required(VERSION 2.8...3.15)
project(edumper-client LANGUAGES C CXX)

add_executable(${PROJECT_NAME}) 
target_sources(${PROJECT_NAME} PRIVATE source/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE includes)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)