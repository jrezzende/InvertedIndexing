cmake_minimum_required(VERSION 3.9)
project(structures CXX)

set(STRUCTURES_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(
	STRUCTURES_HEADERS
	array_list.h
)

add_library(structures STATIC ${STRUCTURES_HEADERS})

target_include_directories(structures PUBLIC ${STRUCTURES_SRC_DIR})