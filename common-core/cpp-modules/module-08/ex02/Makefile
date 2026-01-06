NAME = mutatedAbomination

CC = c++
CFLAGS = $(INC) -std=c++98 -Wall -Wextra -Werror 

INC = 

SRCS =	./main.cpp
		
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