# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 18:26:27 by llebugle          #+#    #+#              #
#    Updated: 2025/08/26 19:31:03 by jfranco          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRCS_DIR = srcs
OBJS_DIR = objs

SRCS = 		    srcs/main.cpp \
			    srcs/core/client_connection.cpp \
			    srcs/core/web_server.cpp \
				srcs/core/connection_manager.cpp \
				srcs/core/server_status.cpp \
				srcs/core/tcp_server.cpp \
			    srcs/handlers/cgi_handler.cpp \
				srcs/handlers/cgi_process.cpp \
			    srcs/handlers/file_streaming.cpp \
				srcs/handlers/response_streamer.cpp \
			    srcs/handlers/request_handler.cpp \
			    srcs/http/http_request.cpp \
			    srcs/http/multipart_parser.cpp \
			    srcs/http/http_response.cpp \
			    srcs/http/mime_types.cpp \
			    srcs/http/request_parser.cpp \
			    srcs/http/utils.cpp \
  				srcs/parsing/ConfigProcessor.cpp \
   				srcs/parsing/GlobalConfig.cpp \
  				srcs/parsing/Location.cpp \
   				srcs/parsing/Server.cpp \
  				srcs/parsing/Node.cpp \
   				srcs/parsing/Validator.cpp \
				srcs/parsing/CgiBin.cpp \
				srcs/lib/socket_guard.cpp \
   				srcs/lib/fd_guard.cpp \
   				srcs/lib/logger.cpp \
  				srcs/lib/file_utils.cpp \
				srcs/lib/stream_buffer.cpp \
				srcs/lib/file_io.cpp \
				srcs/lib/utils.cpp 
			

INC = -I ./includes/

OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.d)

all: $(NAME) 


$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	c++ $(FLAGS) $(INC) -MMD -MP -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

run: all
	./$(NAME) -v

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf build
	rm -f $(NAME)

re: fclean all
test:
	docker build -f tester/Dockerfile -t dockertester .
	docker run -e FULL=1 -ti dockertester
rtest: 
	docker run -e FULL=0 -ti dockertester

	
.PHONY: all clean fclean re

## CMAKE rules ##
cbuild:
	@mkdir -p build
	@cd build && cmake .. && make webserv
#	@cp build/compile_commands.json . 2>/dev/null || true

cbuild-test:
	@mkdir -p build
	@cd build && cmake .. && make test
	
cclean:
	@rm -rf build
# @rm -f compile_commands.json

cdev: cbuild
	@./build/webserv $(filter-out $@,$(MAKECMDGOALS)) -v

crun: cbuild
	@./build/webserv $(filter-out $@,$(MAKECMDGOALS))

ctest: cbuild-test
	@cd build && ./webserv_test

valgrind: cbuild
	@cd build && valgrind ./webserv
.PHONY: cmake-build cmake-clean cmake-run
