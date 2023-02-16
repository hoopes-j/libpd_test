




set(PROJECT_NAME libpd_test)
project(${PROJECT_NAME})

cmake_minimum_required(VERSION 3.24)

set(CXX_STANDARD_REQUIRED 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# RtAudio needs this compiler define  -D__MACOSX_CORE__
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -D__MACOSX_CORE__ -DLIBPD_USE_STD_MUTEX -DHAVE_UNISTD_H"
)

if (APPLE)
  set(SOLIB_EXT dylib)
endif()
if (UNIX AND NOT APPLE)
  set(SOLIB_EXT so)
endif()

set(LIBPD_DIR "/Users/jason/code/libraries/libpd")
set(LIBPD "${LIBPD_DIR}/libs/libpd.${SOLIB_EXT}")


file (GLOB SOURCES "src/*.cpp" "core/*.cpp")
add_executable( ${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE core)

file (GLOB INCLUDE "${LIBPD_DIR}/libpd_wrapper" "${LIBPD_DIR}/libpd_wrapper/util" 
"${LIBPD_DIR}/pure-data/src" "${LIBPD_DIR}/cpp")


target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPD_DIR}/libpd_wrapper)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPD_DIR}/libpd_wrapper/util)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPD_DIR}/pure-data/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPD_DIR}/cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBPD})
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

# Link CoreAudio
find_library(CoreAudioFwk CoreAudio REQUIRED)
message(STATUS "found CoreAudioFwk=${CoreAudioFwk}")
target_link_libraries(${PROJECT_NAME} PUBLIC "${CoreAudioFwk}")

# link CoreFoundation Framework
find_library(CoreFoundationFwk CoreFoundation REQUIRED)
message(STATUS "found CoreFoundationFwk=${CoreFoundationFwk}")
target_link_libraries(${PROJECT_NAME} PUBLIC "${CoreFoundationFwk}")




