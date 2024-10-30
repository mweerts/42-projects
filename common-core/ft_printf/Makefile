BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = libftprintf.a
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 	ft_printf.c \
		ft_parsing.c \
		ft_utils.c \
		ft_print_chr.c \
		ft_print_str.c \
		ft_print_int.c \
		ft_print_uint.c \
		ft_print_hex.c \
		ft_print_ptr.c \
		ft_print_percent.c 
OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a
DIR_LIBFT = ./libft/

all: $(LIBFT) $(NAME)

$(LIBFT):
			@make -C $(DIR_LIBFT) --no-print-directory

$(NAME) : 	$(OBJS)
			@cp $(LIBFT) ./$(NAME)
			@echo "$(COLOUR_GREEN)$(BOLD)[FT_PRINTF] Creating libftprintf.a$(COLOUR_END)"
			@ar rs $(NAME) $(OBJS)

%.o:%.c
		@echo "$(COLOUR_GREEN)[FT_PRINTF] Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) -c $< -o $@

clean: 
		@make clean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FT_PRINTF] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS)

fclean: clean
		@echo "$(COLOUR_RED)$(BOLD)[LIBFT] Deleting \"libft.a\"$(COLOUR_END)"
		@rm -rf $(LIBFT)
		@echo "$(COLOUR_RED)$(BOLD)[FT_PRINTF] Deleting \"libftprintf.a\"$(COLOUR_END)"
		@rm -rf $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean re bonus
