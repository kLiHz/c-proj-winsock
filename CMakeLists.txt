cmake_minimum_required(VERSION 3.13)
project(C-Project-of-Winsock)

add_executable(makebin makebin.c score.h utils.h)
add_executable(readbin readbin.c score.h)

add_executable(server server.c score.h utils.h)
add_executable(client client.c score.h utils.h)

target_link_libraries(server ws2_32)
target_link_libraries(client ws2_32)

add_custom_command(
	TARGET makebin POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/src.csv
			${CMAKE_CURRENT_BINARY_DIR}/src.csv
)
