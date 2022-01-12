cmake_minimum_required(VERSION 3.10)

project(uldl VERSION 0.1 DESCRIPTION "uLambda Debug Library - Nearly-empty C++ library to help debugging some shady code")

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(uldl SHARED
	src/runtime.cpp
	src/raw_tracker.cpp
)

include_directories(inc)

install(TARGETS uldl DESTINATION lib64)
install(FILES inc/uldl.hpp DESTINATION include/uldl)