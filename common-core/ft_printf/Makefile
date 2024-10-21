BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = libftprintf.a
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 	
LIBFT = ./libft
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			@make -C $(LIBFT) --no-print-directory
			@echo "$(COLOUR_GREEN)$(BOLD)[FT_PRINTF] Creating libftprintf$(COLOUR_END)"
			@ar rcs $(NAME) $(OBJS)

%.o:%.c
		@echo "$(COLOUR_GREEN)[FT_PRINTF] Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) -c $< -o $@

clean: 
		@make clean -C $(LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[FT_PRINTF] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS)

fclean: clean
		@echo "$(COLOUR_RED)$(BOLD)[LIBFT] Deleting \"libft.a\"$(COLOUR_END)"
		@rm -rf $(LIBFT)/libft.a
		@echo "$(COLOUR_RED)$(BOLD)[FT_PRINTF] Deleting \"libftprintf.a\"$(COLOUR_END)"
		@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
