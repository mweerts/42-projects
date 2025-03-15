BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = philo
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 	main.c \
		init.c \
		utils.c \
		philo.c \
		monitoring.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			@echo "$(COLOUR_GREEN)$(BOLD)[PHILOSOPHERS] Creating philo$(COLOUR_END)"
			@$(CC) $(CFLAGS) $(OBJS) -pthread -o $(NAME)

%.o:%.c
		@echo "$(COLOUR_GREEN)Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) -I . -pthread -c $< -o $@

clean: 
		@echo "$(COLOUR_RED)$(BOLD)[PHILOSOPHERS] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS)

fclean: clean
		@echo "$(COLOUR_RED)$(BOLD)[PHILOSOPHERS] Deleting \"$(NAME)\"$(COLOUR_END)"
		@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re