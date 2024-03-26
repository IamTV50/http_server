cmake_minimum_required(VERSION 3.27)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp
        src/Parser.cpp
        include/Parser.h
        include/HttpRequestStruct.h
        src/HttpServer.cpp
        include/HttpServer.h
        include/ErrorMessageException.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
