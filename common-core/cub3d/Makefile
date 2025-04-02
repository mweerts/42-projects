# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 19:03:36 by llebugle          #+#    #+#              #
#    Updated: 2025/03/29 22:16:57 by maxweert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cub3d

# -----------------------------------Colors------------------------------------

RED		=\033[0;31m
GREEN	=\033[0;32m
YELLOW	=\033[1;33m
BLUE	=\033[0;34m
ORANGE	=\033[38;2;255;165;0m
RESET	=\033[0m

# Compiler and compilation flags
CC		= cc
CFLAGS	= -Wall -Wextra -Wunused-function -Wunused-variable -g -O1
VALGRIND =
MLX_LIB = -L $(MLX_DIR) -lmlx
MLX_FLAGS = -L./minilibx-linux/ -lmlx_Linux -lXext -lX11 -lm -lz
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
	VALGRIND += valgrind --leak-check=full --show-leak-kinds=all
endif

SRC_PATH = ./srcs/
OBJ_PATH = ./objs/
INC_PATH = ./includes/

#############################
#		SOURCE FILES		#
#############################

SRC		= 	main.c \

SRC		= 	main.c \
			parser/config.c \
			parser/parse_file.c \
			parser/parse_map.c \
			parser/textures.c \
			misc/error.c \
			misc/debug.c \
			misc/clean.c \
			misc/utils.c \
			game/init.c \
			game/events.c \
			game/move.c \
			game/portal.c \
			game/teleport.c \
			game/endgame.c \
			display/animation.c \
			display/init.c \
			display/render.c \
			display/draw_line.c \
			display/draw_texture.c \
			display/draw_mouse_pointer.c \
			display/raycasting.c \
			display/display_map.c \
			display/minimap.c \
			display/texture.c \
			display/hud.c \
			display/utils.c \
			display/draw_pixel.c \
			display/background.c
			
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))
INC		= -I $(INC_PATH) -I $(LIBFT_PATH) -I $(MLX_DIR)

LIBFT_PATH = ./libft/
LIBFT = ./libft/libft.a
MLX_LIB = -L $(MLX_DIR) -lmlx

all:  $(OBJ_PATH) $(LIBFT) $(MLX_PATH) $(NAME) 

run : all
	@./$(NAME) test_maps/valid/map.cub

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(OBJ_PATH)/parser
	mkdir -p $(OBJ_PATH)/game
	mkdir -p $(OBJ_PATH)/misc
	mkdir -p $(OBJ_PATH)/display

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LIB) $(MLX) -o $@ $(INC)

$(MLX_PATH):
	@make -C $(MLX_DIR) --no-print-directory
	
# Libft rule
$(LIBFT):
	make -C $(LIBFT_PATH)

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_PARSER)
	make -C $(LIBFT_PATH) fclean

re: fclean all

#############################
#			TESTS			#
#############################

TEST_DIR = tests
TEST_PARSER = tests/parser

parser: $(LIBFT) $(OBJ_PATH) $(filter-out $(OBJ_PATH)main.o, $(OBJS))
	@mkdir -p $(TEST_DIR)
	@mkdir -p $(OBJ_PATH)/testing
	@$(CC) $(CFLAGS) -c $(TEST_DIR)/test_parser.c -o $(OBJ_PATH)testing/test_parser.o $(INC) -D BONUS=0
	@$(CC) $(CFLAGS) $(filter-out $(OBJ_PATH)main.o, $(OBJS)) $(OBJ_PATH)testing/test_parser.o -o $(TEST_PARSER) $(INC) $(LIBFT) $(MLX_LIB) $(MLX)
	@echo "$(BLUE)parser program compiled successfully!$(RESET)"
	@echo "$(GREEN)executing parser$(RESET)"
	
test: parser
	@chmod +x $(TEST_DIR)/test_maps.sh
	@$(TEST_DIR)/test_maps.sh

# $(if $(VALGRIND), $(VALGRIND))
#	$(VALGRIND) ./parser ./maps/map.cub

.PHONY: all re clean fclean run parser