set(PROJECT_NAME main)
project(${PROJECT_NAME})

cmake_minimum_required(VERSION 3.24)

set(CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# RtAudio needs this compiler define  -D__MACOSX_CORE__
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -D__MACOSX_CORE__")


file (GLOB SOURCES "src/*.cpp" "johnson/*.cpp")
add_executable( ${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE johnson)

# Link CoreAudio
find_library(CoreAudioFwk CoreAudio REQUIRED)
message(STATUS "found CoreAudioFwk=${CoreAudioFwk}")
target_link_libraries(${PROJECT_NAME} PUBLIC "${CoreAudioFwk}")

# link CoreFoundation Framework
find_library(CoreFoundationFwk CoreFoundation REQUIRED)
message(STATUS "found CoreFoundationFwk=${CoreFoundationFwk}")
target_link_libraries(${PROJECT_NAME} PUBLIC "${CoreFoundationFwk}")




