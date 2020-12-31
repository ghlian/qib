cmake_minimum_required (VERSION 3.1.0)

set (PROGRAM_NAME	"main_test")
set (PROGRAM_VER    "0.0.1")
set (KX_VER         "3")
set (TWS_VER        "9.71.01")
set (BINARY_NAME	"${PROGRAM_NAME}.${PROGRAM_VER}")

set(PROJECT_NAME main_test)
project(${PROJECT_NAME})

set(QLIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
#set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions (-DKXVER=3 -DIB_USE_STD_STRING)

#set_target_properties (${BINARY_NAME} PROPERTIES
#	PREFIX ""
#	CXX_STANDARD 14
#	CXX_STANDARD_REQUIRED ON)


include_directories(${QLIBDIR}/src
		"${QLIBDIR}/bin/include"
		"${QLIBDIR}/third_party/kx"
		"${QLIBDIR}/third_party/ib/IBJts/source/PosixClient/Shared"
		"${QLIBDIR}/third_party/ib/IBJts/source/PosixClient/src")

#link_directories(../bin)
#link_libraries(${QLIBDIR}/bin/qib.0.0.1.so ${QLIBDIR}/third_party/kx/k.so)
#link_libraries(${QLIBDIR}/bin)
#LINK_DIRECTORIES(${QLIBDIR}/bin)

#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCHFLAGS}")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCHFLAGS}")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fpic -std=gnu99")

# add_library(main_test SHARED main_test.cpp) ## this is to compile a dynamics shared file .so

aux_source_directory(${QLIBDIR}/src QLIB_SRC/*cpp)
## below is to compile to executible file
add_executable(${PROJECT_NAME} main_test.cpp ${QLIB_SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QLIBDIR}/bin/qib.0.0.1.so ${QLIBDIR}/third_party/kx/c.o )
target_link_libraries( ${PROJECT_NAME} pthread )

#


