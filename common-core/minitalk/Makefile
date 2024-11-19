BOLD = \033[1m
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

CLIENT = client
SERVER = server

CC = cc 
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address

SRCS_CLIENT =	srcs/client.c 

SRCS_SERVER =	srcs/server.c 

SRCSDIR = ./srcs/
INCLUDES = ./includes

OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)
OBJS_SERVER = $(SRCS_SERVER:.c=.o)

DIR_LIBFT = ./libft
LIBFT = $(DIR_LIBFT)/libft.a

all: $(LIBFT) $(CLIENT) $(SERVER)

$(LIBFT):
			@make -C $(DIR_LIBFT) --no-print-directory

$(CLIENT) :		$(OBJS_CLIENT)
				@echo "$(COLOUR_GREEN)$(BOLD)[MINITALK] Creating $(CLIENT)$(COLOUR_END)"
				$(CC) $(CFLAGS) $(OBJS_CLIENT) -L$(DIR_LIBFT) -lft -I $(INCLUDES) -o $(CLIENT)

$(SERVER) : 	$(OBJS_SERVER)
				@echo "$(COLOUR_GREEN)$(BOLD)[MINITALK] Creating $(SERVER)$(COLOUR_END)"
				@$(CC) $(CFLAGS) $(OBJS_SERVER) -L$(DIR_LIBFT) -lft -I $(INCLUDES) -o $(SERVER)

%.o:%.c
		@echo "$(COLOUR_GREEN)[MINITALK] Compiling $<$(COLOUR_END)"
		@$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

clean: 
		@make clean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[MINITALK] Deleting objects files$(COLOUR_END)"
		@rm -rf $(OBJS_CLIENT) $(OBJS_SERVER)

fclean: 
		@rm -rf $(OBJS_CLIENT) $(OBJS_SERVER)
		@echo "$(COLOUR_RED)$(BOLD)[MINITALK] Deleting objects files$(COLOUR_END)"
		@make fclean -C $(DIR_LIBFT) --no-print-directory
		@echo "$(COLOUR_RED)$(BOLD)[MINITALK] Deleting \"$(CLIENT)\"$(COLOUR_END)"
		@echo "$(COLOUR_RED)$(BOLD)[MINITALK] Deleting \"$(SERVER)\"$(COLOUR_END)"
		@rm -rf $(CLIENT) $(SERVER)

re: fclean all

.PHONY: all clean fclean re