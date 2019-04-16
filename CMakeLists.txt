cmake_minimum_required (VERSION 3.1.0)

set (PROGRAM_NAME	"qib")
set (PROGRAM_VER    "0.0.2")

set (BINARY_NAME	"${PROGRAM_NAME}.${PROGRAM_VER}")

project (${BINARY_NAME} CXX C)

set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file (GLOB_RECURSE QIB_SRC
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/cppclient/*.cpp")
add_library (${BINARY_NAME} SHARED ${QIB_SRC})

include_directories ("${CMAKE_BINARY_DIR}/include"
  "${CMAKE_SOURCE_DIR}/cppclient")

set_target_properties (${BINARY_NAME} PROPERTIES
  PREFIX ""
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON)

set_property(TARGET ${BINARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties (${BINARY_NAME} PROPERTIES LINK_FLAGS "-shared -Wl,-rpath,.")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -ggdb3 -gdwarf-2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -ggdb3 -gdwarf-2")
