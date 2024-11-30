BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = push_swap
BONUS_NAME = checker

CC = cc 
CFLAGS = -Wall -Wextra -Werror

SRC = 	push_swap.c \
		parsing.c \
		parsing_utils.c \
		utils.c \
		stack_swap.c \
		stack_push.c \
		stack_rotate.c \
		stack_reverse_rotate.c \
		quicksort.c \
		sort_three.c \
		move.c \
		chunk.c \
		optimize.c

BONUS_SRC =	checker_bonus.c \
			parsing.c \
			parsing_utils.c \
			utils.c \
			stack_swap.c \
			stack_push.c \
			stack_rotate.c \
			stack_reverse_rotate.c 

SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC)) 	
BONUS_SRCS = $(addprefix $(SRCSDIR), $(BONUS_SRC))
INCLUDES = ./includes

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

DIR_LIBFT = ./libft
LIBFT = $(DIR_LIBFT)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
			@make -C $(DIR_LIBFT) --no-print-directory

$(BONUS_NAME) :	$(BONUS_OBJS)
				@cp $(LIBFT) ./$(BONUS_NAME)
				@echo "$(COLOUR_GREEN)$(BOLD)[FDF] Creating $(BONUS_NAME)$(COLOUR_END)"
				@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -I $(INCLUDES) -o $(BONUS_NAME)

$(NAME) : 	$(OBJS)
			@cp $(LIBFT) ./$(NAME)
			@echo "$(COLOUR_GREEN)$(BOLD)[FDF] Creating $(NAME)$(COLOUR_END)"
			@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -I $(INCLUDES) -o $(NAME)

%.o:%.c
		@echo "$(COLOUR_GREEN)[FDF] Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) $(MLX_FLAGS) -I $(INCLUDES) -c $< -o $@

clean: 
		@make clean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS) $(BONUS_OBJS)

fclean: 
		@rm -rf $(OBJS) $(BONUS_OBJS)
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting objects files$(COLOUR_END)"
		@make fclean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting \"$(NAME)\"$(COLOUR_END)"
		@echo "$(COLOUR_RED)$(BOLD)[FDF] Deleting \"$(BONUS_NAME)\"$(COLOUR_END)"
		@rm -rf $(NAME) $(BONUS_NAME)

bonus: $(LIBFT) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus