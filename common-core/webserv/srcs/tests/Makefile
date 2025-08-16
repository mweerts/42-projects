# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 18:26:27 by llebugle          #+#    #+#              #
#    Updated: 2025/07/23 22:21:06 by llebugle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = tests

FLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = -I ../../includes

SRC_TEST_REQUEST =  test_request.cpp ../http/HttpRequest.cpp ../lib/logger.cpp
test-request: 
	c++ $(FLAGS) -o test-request $(SRC_TEST_REQUEST) $(INCLUDES)

run: test-request
	./test-request

clean:
	rm -f *.o

fclean: clean
	rm -f test_request

.PHONY: clean test-request
