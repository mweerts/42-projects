# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 18:26:27 by llebugle          #+#    #+#              #
#    Updated: 2025/06/25 18:26:32 by llebugle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS_DIR = srcs
OBJS_DIR = objs
SRCS = $(shell find $(SRCS_DIR) -name "*.cpp")

OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.d)

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	c++ $(FLAGS) -MMD -MP -c $< -o $@

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
	@cd build && cmake .. && make
#	@cp build/compile_commands.json . 2>/dev/null || true

cclean:
	@rm -rf build
# @rm -f compile_commands.json

crun: cmake-build
	@cd build && ./webserv

.PHONY: cmake-build cmake-clean cmake-run