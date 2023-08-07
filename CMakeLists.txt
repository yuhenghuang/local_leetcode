cmake_minimum_required(VERSION 3.16.3)

project(local_leetcode VERSION 0.9.4.2)

# check clang and its version
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
  message(SEND_ERROR "Please choose clang as the compiler")
endif()

if(CLANG_VERSION_MAJOR VERSION_GREATER 11)
  message(SEND_ERROR "Clang major version should be at least 11")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")

set(CMAKE_CXX_FLAGS_DEBUG "-g -D_LIBCPP_DEBUG=1")

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
set(CMAKE_SHARED_LINKER_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")


option(USE_LIBC++ "need to build with libc++ on non-macos machine?" OFF)

if(USE_LIBC++)
  set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "-stdlib=libc++ ${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
  set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ ${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

# for output .lib (or .dll.a) on windows
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

# read source files in ./src to SRC
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_library(local_leetcode SHARED ${SRC})

# save shared library (.dll) in bin/ in windows
if(WIN32)
  set_target_properties(local_leetcode 
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
endif()

option(BUILD_EXAMPLES "need to build examples?" OFF)

if(BUILD_EXAMPLES)
  include(CTest)

  # print output
  list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")

  set(EXAMPLE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/examples)

  # read example source files
  aux_source_directory(${EXAMPLE_SOURCE_DIR} EXAMPLE_SRC)

  # compile examples
  foreach(example_source_file ${EXAMPLE_SRC})
    get_filename_component(example_file ${example_source_file} NAME_WE)

    add_executable(${example_file} ${example_source_file})

    target_link_libraries(${example_file} local_leetcode)

    add_test(NAME ${example_file} COMMAND ${example_file} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
  endforeach()

  # copy example source and input files
  add_custom_command(
    TARGET 01_solution PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${EXAMPLE_SOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  )

endif() # BUILD_EXAMPLE

unset(USE_LIBC++ CACHE)
unset(BUILD_EXAMPLES CACHE) # important: without this line the cache will affect the default value
