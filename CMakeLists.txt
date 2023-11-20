cmake_minimum_required(VERSION 3.22.1)
project(aasl VERSION 0.0.0)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME}play ./demo/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)

find_package(ALSA REQUIRED)                                    
if (ALSA_FOUND)                                                
    include_directories(${ALSA_INCLUDE_DIRS})                  
    target_link_libraries (${PROJECT_NAME} ${ALSA_LIBRARIES})      
endif(ALSA_FOUND)  

target_link_libraries(${PROJECT_NAME}play aasl)
