NAME = do-op
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = 	ft_do_op.c ft_operations.c ft_utils.c
SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC))
INC = ./includes
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			$(CC) $(OBJS) -o $(NAME)

%.o:%.c
		$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all