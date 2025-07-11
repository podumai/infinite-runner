cmake_minimum_required(VERSION 3.23)
project(INFINITE-RUNNER VERSION 0.0.1
                        LANGUAGES CXX)
find_package(raylib REQUIRED)
#add_executable(${PROJECT_NAME} src/main.cpp)
#target_compile_options(${PROJECT_NAME} PUBLIC -std=c++23)
#target_link_libraries(${PROJECT_NAME} PUBLIC -lraylib)
#target_include_directories(${PROJECT_NAME} PUBLIC include)
add_executable(${PROJECT_NAME})
string(TOLOWER  "${PROJECT_NAME}-HEADERS" header_file_set)
target_sources(
    ${PROJECT_NAME}
      PRIVATE
        FILE_SET header_file_set
        TYPE HEADERS
        BASE_DIRS
          ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
          ${CMAKE_CURRENT_SOURCE_DIR}/include/raygui.h
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
target_link_libraries(
    ${PROJECT_NAME}
      PRIVATE
        -lraylib
)
target_compile_features(
    ${PROJECT_NAME}
      PRIVATE
        cxx_std_23
      PUBLIC
        cxx_std_17
)