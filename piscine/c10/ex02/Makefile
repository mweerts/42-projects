NAME = ft_tail
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = 	ft_tail.c \
		ft_putstr_error.c \
		ft_list.c \
		ft_check_args.c \
		ft_atoi.c \
		ft_stack.c
SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC))
INC = ./includes
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.c
		$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all