# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 19:03:36 by llebugle          #+#    #+#              #
#    Updated: 2025/03/13 19:03:40 by llebugle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cub3D

# -----------------------------------Colors------------------------------------

RED		=\033[0;31m
GREEN	=\033[0;32m
YELLOW	=\033[1;33m
BLUE	=\033[0;34m
ORANGE	=\033[38;2;255;165;0m
RESET	=\033[0m

# Compiler and compilation flags
CC		= cc
CFLAGS	= -Wextra -Wall -g
VALGRIND =

ifeq ($(shell uname), Linux)
	MLX_DIR = ./minilibx-linux
	MLX_PATH = minilibx-linux/libmlx.a
	MLX =  -L /usr/lib -lXext -lX11 -lm -lz
	MLX_OBJ = -I/usr/include -Imlx_linux -O3
else
	MLX_DIR = ./mlx
	MLX_PATH = mlx/libmlx.a
	MLX = -lmlx -Lmlx -framework OpenGl -framework AppKit
	MLX_OBJ = -Imlx -c
endif

ifeq ($(MODE), debug)
	CFLAGS	+= -fsanitize=address -g -D DEBUG=true
else ifeq ($(MODE), valgrind)
	VALGRIND += valgrind --leak-check=full --show-leak-kinds=all --suppressions=leaks.supp
endif

SRC_PATH = ./srcs/
OBJ_PATH = ./objs/
INC_PATH = ./includes/

SRC		= 	main.c \

OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))
INC		= -I $(INC_PATH) -I $(LIBFT_PATH)

LIBFT_PATH = ./libft/
LIBFT = ./libft/libft.a

all: $(OBJ_PATH) $(LIBFT) $(NAME)


run : all
	@./$(NAME) map.cub

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(OBJ_PATH)/parser
	mkdir -p $(OBJ_PATH)/misc

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LIB) $(MLX) -o $@ $(INC)

# Libft rule
$(LIBFT):
	make -C $(LIBFT_PATH)

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
#	rm -rf $(TESTER_DIR)
	make -C $(LIBFT_PATH) fclean

re: fclean all

#############################
#			TESTS			#
#############################

TEST_DIR = tests

parser: $(OBJ_PATH) $(filter-out $(OBJ_PATH)main.o, $(OBJS))
	mkdir -p $(TEST_DIR)
	mkdir -p $(OBJ_PATH)/testing
	$(CC) $(CFLAGS) -c $(TEST_DIR)/test_parser.c -o $(OBJ_PATH)testing/test_parser.o $(INC) $(MLX_LIB) $(MLX)
	$(CC) $(CFLAGS) $(filter-out $(OBJ_PATH)main.o, $(OBJS)) $(OBJ_PATH)testing/test_parser.o -o parser $(INC) $(LIBFT) $(MLX_LIB) $(MLX)
	@echo "$(BLUE)parser program compiled successfully!$(RESET)"
	$(if $(VALGRIND), $(VALGRIND))
	$ ./parser

.PHONY: all re clean fclean run parser