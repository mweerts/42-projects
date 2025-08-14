cmake_minimum_required(VERSION 3.20)
project(webserv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++98")

include_directories(includes)


set(SOURCES
    srcs/main.cpp
    srcs/core/web_server.cpp
    srcs/core/client_connection.cpp
	srcs/core/connection_manager.cpp
	srcs/core/tcp_server.cpp
	srcs/core/response_streamer.cpp
	
    srcs/handlers/cgi_handler.cpp
    srcs/handlers/cgi_process.cpp
    srcs/handlers/file_streaming.cpp
    srcs/handlers/RequestHandler.cpp
	
    # srcs/http/HttpRequest.cpp
    srcs/http/HttpRequest.cpp
    srcs/http/MultipartParser.cpp
    srcs/http/HttpResponse.cpp
    srcs/http/MimeTypes.cpp
    srcs/http/RequestParser.cpp
    srcs/http/utils.cpp
    
    srcs/parsing/ConfigProcessor.cpp
    srcs/parsing/GlobalConfig.cpp
    srcs/parsing/Location.cpp
    srcs/parsing/Server.cpp
    srcs/parsing/Node.cpp
    srcs/parsing/Validator.cpp
    srcs/parsing/CgiBin.cpp
    
    srcs/lib/socket_guard.cpp
    srcs/lib/logger.cpp
    srcs/lib/file_utils.cpp
	srcs/lib/stream_buffer.cpp
	srcs/lib/file_io.cpp
    srcs/lib/utils.cpp
    srcs/tests.cpp
)

add_executable(webserv ${SOURCES})

# Test executable with TEST flag
add_executable(webserv_test ${SOURCES})
target_compile_definitions(webserv_test PRIVATE TEST=1)
target_link_libraries(webserv_test ${LIBS})

add_custom_target(run
    COMMAND ./webserv
    DEPENDS webserv
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(test
    COMMAND ./webserv_test
    DEPENDS webserv_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests..."
)

