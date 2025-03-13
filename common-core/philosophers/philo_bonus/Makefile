BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = philo
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 	main_bonus.c \
		init_bonus.c \
		utils_bonus.c \
		philo_bonus.c \
		monitoring_bonus.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			@echo "$(COLOUR_GREEN)$(BOLD)[PHILOSOPHERS] Creating philo$(COLOUR_END)"
			@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

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