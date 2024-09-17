NAME = libft.a
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRC = 	ft_putchar.c \
		ft_swap.c \
		ft_putstr.c \
		ft_strlen.c \
		ft_strcmp.c
SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC)) 
INC = ./includes
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			ar rc $(NAME) $(OBJS)
			ranlib $(NAME)

%.o:%.c
		$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean: 
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all