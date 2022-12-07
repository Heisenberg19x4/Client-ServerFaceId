cmake_minimum_required(VERSION 3.24)
project(FaceID C)

set(CMAKE_C_STANDARD 11)

add_executable(FaceIDServer Server/server.c)
add_executable(FaceIDClient Client/client.c)
