# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 18:26:27 by llebugle          #+#    #+#              #
#    Updated: 2025/07/19 02:09:42 by llebugle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS_DIR = srcs
OBJS_DIR = objs
SRCS = $(shell find $(SRCS_DIR) -name "*.cpp" ! -path "srcs/parsing/test/*")


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
	./$(NAME)

clean:
	rm -rf $(OBJS_DIR)
	rm -rf build

fclean: clean
	rm -f $(NAME)

re: fclean all
	
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

crun: cbuild
	@./build/webserv $(filter-out $@,$(MAKECMDGOALS))

ctest: cbuild-test
	@cd build && ./webserv_test

valgrind: cbuild
	@cd build && valgrind ./webserv
.PHONY: cmake-build cmake-clean cmake-run
