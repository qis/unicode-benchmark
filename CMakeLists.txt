cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(benchmark VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if(WIN32)
  add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

set(sources src/common.hpp src/common.cpp src/main.cpp)
set(include_paths)
set(libraries)

find_path(UTFCPP_INCLUDE_DIR utf8.h)
if(UTFCPP_INCLUDE_DIR)
  list(APPEND sources src/utfcpp.cpp)
  list(APPEND include_paths ${UTFCPP_INCLUDE_DIR})
else()
  message(STATUS "Missing dependency: utfcpp (utf8.h)")
endif()

find_path(UTF8H_INCLUDE_DIR utf8h/utf8.h)
if(UTF8H_INCLUDE_DIR)
  list(APPEND sources src/utf8h.cpp)
  list(APPEND include_paths ${UTF8H_INCLUDE_DIR})
else()
  message(STATUS "Missing dependency: utf8h (utf8h/utf8.h)")
endif()

find_path(UTF8PROC_INCLUDE_DIR utf8proc.h)
if(UTF8PROC_INCLUDE_DIR)
  list(APPEND sources src/utf8proc.cpp)
  list(APPEND include_paths ${UTF8PROC_INCLUDE_DIR})
  find_library(UTF8PROC_LIBRARY NAMES utf8proc)
  if(UTF8PROC_LIBRARY)
    list(APPEND libraries ${UTF8PROC_LIBRARY})
  else()
    message(WARNING "Missing library for dependency: utf8proc (utf8proc)")
  endif()
else()
  message(STATUS "Missing dependency: utf8proc (utf8proc.h)")
endif()

find_path(UTFZ_INCLUDE_DIR utfz.h)
if(UTFZ_INCLUDE_DIR)
  list(APPEND sources src/utfz.cpp)
  list(APPEND include_paths ${UTFZ_INCLUDE_DIR})
  find_library(UTFZ_LIBRARY NAMES utfz)
  if(UTFZ_LIBRARY)
    list(APPEND libraries ${UTFZ_LIBRARY})
  else()
    message(WARNING "Missing library for dependency: utfz (utfz)")
  endif()
else()
  message(STATUS "Missing dependency: utfz (utfz.h)")
endif()

add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${include_paths})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libraries})

if(WIN32)
  configure_file(src/main.rc.in ${CMAKE_CURRENT_BINARY_DIR}/src/main.rc)
  target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/main.rc src/main.manifest)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_link_libraries(${PROJECT_NAME} PRIVATE tbb tbbmalloc)
endif()

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

find_package(benchmark CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

if(WIN32)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ DESTINATION bin
    FILES_MATCHING PATTERN "*.dll" PATTERN "CMakeFiles" EXCLUDE PATTERN "res" EXCLUDE PATTERN "src" EXCLUDE)
endif()

add_custom_target(run COMMAND $<TARGET_FILE:benchmark>
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  DEPENDS benchmark USES_TERMINAL)
