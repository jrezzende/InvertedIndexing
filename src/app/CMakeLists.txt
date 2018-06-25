cmake_minimum_required(VERSION 3.9)
project(app CXX)

set(APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(
	APP_HEADERS
	app.h
)

set(
	APP_SOURCES
	app.cpp
	main.cpp
)

add_executable(app ${APP_HEADERS} ${APP_SOURCES})

target_include_directories(app PUBLIC ${APP_SRC_DIR})

