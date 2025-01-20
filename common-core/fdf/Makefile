BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = fdf
CC = cc 

CFLAGS = -Wall -Wextra -Werror 
MLX_FLAGS = -L./minilibx-linux/ -lmlx_Linux -lXext -lX11 -lm -lz
INC_FLAGS = -I./includes -I./libft -I./minilibx-linux

SRC =	fdf.c \
		init.c \
		utils.c \
		events.c \
		draw.c \
		draw_line.c \
		parsing.c \
		projection.c \
		transformations.c 

SRCS = $(addprefix ./srcs/, $(SRC))
OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a
DIR_LIBFT = ./libft/

all: $(LIBFT) $(NAME)

$(LIBFT):
			@make -C $(DIR_LIBFT) --no-print-directory

$(NAME) : 	$(OBJS)
			@echo "$(COLOUR_GREEN)$(BOLD)[FDF] Creating $(NAME)$(COLOUR_END)"
			@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_FLAGS) $(INC_FLAGS) -o $(NAME)

%.o:%.c
		@echo "$(COLOUR_GREEN)[FDF] Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

clean: 
		@make clean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS)

fclean: 
		@rm -rf $(OBJS)
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting objects files$(COLOUR_END)"
		@make fclean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting \"$(NAME)\"$(COLOUR_END)"
		@rm -rf $(NAME)

re: fclean all

exec: all
		./fdf

.PHONY: all clean fclean re