cmake_minimum_required(VERSION 3.17)
project(deception)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
find_package(Boost
		REQUIRED
		COMPONENTS program_options)
add_library(deception-core
        lib/core/Concepts.h
        lib/core/Table.cc
)
add_executable(deception-interpreter
		cmd/simple/deception.cc
		)
target_link_libraries(deception-interpreter
		Boost::program_options
        deception-core
)
install(TARGETS deception-interpreter
        DESTINATION bin)

target_include_directories(deception-interpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_include_directories(deception-core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)
