cmake_minimum_required(VERSION 3.17)
project(deception)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
find_package(Boost
		REQUIRED
		COMPONENTS program_options)

add_executable(deception
		cmd/simple_interpreter/deception.cc
		)
target_link_libraries(deception
		Boost::program_options)
install(TARGETS deception DESTINATION bin)

#target_include_directories(deception PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
