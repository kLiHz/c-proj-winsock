project(C-Project-of-Winsock)

add_executable(makebin makebin.c score.h utils.h)
add_executable(readbin readbin.c score.h)

add_executable(server server.c score.h utils.h)
add_executable(client client.c score.h utils.h)

target_link_libraries(server ws2_32)
target_link_libraries(client ws2_32)
