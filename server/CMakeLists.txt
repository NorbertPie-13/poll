cmake_minimum_required(VERSION 3.15)

project(poll_server LANGUAGES C)

string(CONCAT CMAKE_CHECKS
"-altera*,"
"-cert-dcl03-c,"
"-misc-static-assert,"
"-llvm-include-order,"
"-llvmlibc-*,"
"-hicpp-*,"
"-concurrency-mt-unsafe,"
"-bugprone-easily-swappable-parameters,"
"-cppcoreguidelines-*,"
"-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling"
)
      
if(CMAKE_BUILD_TYPE MATCHES Debug)
        message(STATUS "*** building in DEBUG mode ***")
        enable_testing()
        add_compile_options(-std=c99 -Wall -Wextra -pedantic -Werror -O3
                                -fsanitize=address -D_POSIX_C_SOURCE=200112L -g)
        link_libraries(-fsanitize=address)
        add_compile_definitions(DEBUG)
        set(CMAKE_C_CLANG_TIDY
                "clang-tidy" "-checks=${CMAKE_CHECKS}" "-warnings-as-errors=*"
        )
elseif(CMAKE_BUILD_TYPE MATCHES Release)
        message(STATUS "*** building in RELEASE mode ***")
        add_compile_options(-std=c99 -Wall -Wextra -pedantic -O3 -D_POSIX_C_SOURCE=200112L)
        add_compile_definitions(NDEBUG)
        set(CMAKE_C_CLANG_TIDY
                "clang-tidy" "-checks=${CMAKE_CHECKS}"
        )
  endif()

  set(SOURCES
  src/poll_main.c
  src/poll_server.c
  src/err_funcs.c
  src/unix_socket.c
  )

set(HEADERS
  include/poll_server.h
  include/err_funcs.h
  include/unix_socket.h
  include/poll_main.h
  )

add_executable(poll_server ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} -lpthread)
      