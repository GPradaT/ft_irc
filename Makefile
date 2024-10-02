NAME	=	ircserv

FILES	=	main

SRCS	=	$(addsuffix .cpp $(FILES))
OBJS	=	$(SRC:.c=.o)

vpath %.cpp srcs

CXX		=	c++ -std=c++98 -Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
