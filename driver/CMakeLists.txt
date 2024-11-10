cmake_minimum_required(VERSION 3.5.0)
project(edumper-driver VERSION 0.1.0 LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/libraries/FindWDK/cmake")
find_package(WDK REQUIRED)

wdk_add_library(ksocket KMDF 1.15 ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ksocket/src/ksocket/ksocket.c ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ksocket/src/ksocket/berkeley.c ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ksocket/src/ksocket/berkeley.h ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ksocket/src/ksocket/ksocket.h)

wdk_add_driver(${PROJECT_NAME} KMDF 1.15 source/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE includes)
target_link_options(${PROJECT_NAME} PUBLIC "/ENTRY:CustomDriverEntry")
target_link_libraries(${PROJECT_NAME} WDK::NETIO ksocket)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)