cmake_minimum_required(VERSION 3.17)
project(deception)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
find_package(Boost
		REQUIRED
		COMPONENTS
        system
)
add_library(deception-core
        lib/core/Interpreter.h
        lib/core/Interpreter.cpp
        lib/core/Table.h
        lib/core/Conclave.h
        lib/core/Codes.cc
        lib/core/Codes.h
        lib/core/Value.h
        lib/core/MemorySpace.cc
        lib/core/MemorySpace.h
)
add_executable(deception-interpreter
		cmd/simple/deception.cc
		)

target_link_libraries(deception-interpreter
        Boost::system
        deception-core
)
install(TARGETS deception-interpreter
        DESTINATION bin)

target_include_directories(deception-interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(deception-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
