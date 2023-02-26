cmake_minimum_required(VERSION 3.15)

project(poll LANGUAGES C)

set(SOURCES
        src/poll_main.c
        src/server.c
        src/poll_server.c
        src/err_funcs.c
        )

set(HEADERS
        include/server.h
        include/poll_server.h
        include/err_funcs.h
        )
        
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wunused-variable -g )

target_link_options(${PROJECT_NAME} PRIVATE  -Wall -Wextra -pedantic -Wunused-variable -g -O3 )
target_link_libraries(${PROJECT_NAME} -lpthread)