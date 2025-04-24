BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

NAME = animal

CC = c++
CFLAGS = -I $(INCLUDES) -g -std=c++98 -Wall -Wextra -Werror 

INCLUDES = ./includes/

SRCS =	./srcs/main.cpp \
		./srcs/Animal.cpp \
		./srcs/Cat.cpp \
		./srcs/Dog.cpp \
		./srcs/WrongAnimal.cpp \
		./srcs/WrongCat.cpp \
		./srcs/Brain.cpp
		
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) :	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:%.cpp
		$(CC) $(CFLAGS) -c $< -o $@

clean: 
		rm -rf $(OBJS)

fclean: 
		rm -rf $(OBJS)
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re