BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = philo
CC = cc 
CFLAGS = -g -pthread -Wall -Wextra -Werror
SRC = 	main.c \
		parsing.c \
		utils.c \
		init.c \
		philo_routine.c \
		monitoring_routine.c

SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC)) 	
INCLUDES = ./includes

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			@echo "$(COLOUR_GREEN)$(BOLD)[PHILOSOPHERS] Creating philo$(COLOUR_END)"
			@$(CC) $(CFLAGS) $(OBJS) -I $(INCLUDES) -o $(NAME)

%.o:%.c
		@echo "$(COLOUR_GREEN)Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

clean: 
		@echo "$(COLOUR_RED)$(BOLD)[PHILOSOPHERS] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS)

fclean: clean
		@echo "$(COLOUR_RED)$(BOLD)[PHILOSOPHERS] Deleting \"$(NAME)\"$(COLOUR_END)"
		@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re